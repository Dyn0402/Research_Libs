/*
 * RatioData.cpp
 *
 *  Created on: Jul 31, 2019
 *      Author: Dylan Neff
 */


#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <dirent.h>

#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <TF1.h>
#include <TLine.h>
#include <TCanvas.h>

#include "RatioData.h"
#include "file_io.h"

using namespace std;

// Structors
RatioData::RatioData(int divisions) {
	divs = divisions;
}

RatioData::~RatioData() {
	// Nothing
}


// Getters
map<int, map<int, int>> RatioData::get_ratio_data() {
	if(!ratio_data_gen) {
		cout << "No ratio data generated. Returning empty map." << endl;
	}
	return ratio_data;
}

map<int, int> RatioData::get_proton_dist() {
	if(!proton_dist_gen) {
		gen_proton_dist();
	}

	return proton_dist;
}


// Setters
void RatioData::set_ratio_data(map<int, map<int, int>> ratio_data_in) {
	ratio_data = ratio_data_in;
	ratio_data_gen = true;
}

void RatioData::set_divs(int divs_in) {
	divs = divs_in;
}


// Doers
void RatioData::read_data_from_file(string file_path) {
	ifstream in_file(file_path);
	string line;
	if(in_file.is_open()) {
		while(getline(in_file,line)) {
			vector<string> line_elements = split_string_by_char(line, data_delimeters[0]);
			vector<string> data_elements = split_string_by_char(line_elements[1], data_delimeters[1]);
			for(string element:data_elements) {
				if(element.size() > 1) {
					vector<string> datum = split_string_by_char(element, data_delimeters[2]);
					ratio_data[stoi(line_elements[0])][stoi(datum[0])] += stoi(datum[1]);
				}
			}
		}
	ratio_data_gen = true;
	}
}


void RatioData::read_ratios_from_dir(string dir_path, int div, int cent) {
	DIR* files_dir = opendir(dir_path.data());
	struct dirent* dp;
	while((dp=readdir(files_dir)) != NULL) {
		string file = dp->d_name;
		vector<string> fields = split_string_by_char(file, file_name_delimeter);
		if(fields[0] == ratios_file_pre) {
			if(stoi(fields[2]) == div) {
				if(stoi(fields[4]) == cent) {
					ifstream in_file(dir_path+file);
					string line;
					if(in_file.is_open()) {
						while(getline(in_file,line)) {
							vector<string> line_elements = split_string_by_char(line, data_delimeters[0]);
							vector<string> data_elements = split_string_by_char(line_elements[1], data_delimeters[1]);
							for(string element:data_elements) {
								if(element.size() > 1) {
									vector<string> datum = split_string_by_char(element, data_delimeters[2]);
									ratio_data[stoi(line_elements[0])][stoi(datum[0])] += stoi(datum[1]);
								}
							}
						}
						ratio_data_gen = true;
					}
				}
			}
		}
	}
}


void RatioData::write_ratios(string unique_suffix, string dir_path, int div, int cent) {
	string out_name = dir_path + ratios_file_pre + file_name_delimeter;
	out_name += ratios_file_fields[0] + file_name_delimeter + to_string(div);
	out_name += file_name_delimeter + ratios_file_fields[1] + file_name_delimeter + to_string(cent);
	out_name += file_name_delimeter + unique_suffix + file_ext;
	ofstream out_file(out_name);
	for(pair<int, map<int, int>> nprotons:ratio_data) {
		out_file << to_string(nprotons.first) << data_delimeters[0];
		for(pair<int, int> bin_protons:nprotons.second) {
			out_file << to_string(bin_protons.first) << data_delimeters[2] << to_string(bin_protons.second) << data_delimeters[1];
		}
		out_file << endl;
	}
	out_file.close();
}

void RatioData::gen_proton_dist() {
	for(pair<int, map<int, int>> proton_per_event:ratio_data) {
		for(pair<int, int> proton_per_div:proton_per_event.second) {
			proton_dist[proton_per_event.first] += proton_per_div.second;
		}
		proton_dist[proton_per_event.first] /= divs;
	}
}


void RatioData::canvas_2d_dist(TFile *out_file, string name, double p_clust) {
	out_file->cd();
	TH2I *hist = ratios_map_to_hist(ratio_data, name);
	TCanvas *can = new TCanvas((name+"_Can").data());
	TF1 *avg_line = new TF1((name+"_avg").data(), ("x/"+to_string(divs)).data(), -0.5, 0.5+(--ratio_data.end())->first);
	TF1 *max_line = new TF1((name+"_max").data(), "x", -0.5, 0.5+(--ratio_data.end())->first);
	max_line->SetLineColor(4);
	can->SetLogz();
	hist->Draw("colz");
	max_line->Draw("same");
	avg_line->Draw("same");

	if(p_clust != -1) {
		int eff_bound = ceil(1.5 / p_clust); // Minimum num protons to get a grouping of 2 (therefore see any effect)
		TLine *eff_line = new TLine(eff_bound, 0, eff_bound, eff_bound);
		eff_line->SetLineColor(kRed);
		eff_line->Draw("same");
	}

	can->Write();

	delete avg_line;
	delete max_line;
	delete hist;
	delete can;
}
