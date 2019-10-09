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
#include <TStyle.h>
#include <TLegend.h>
#include <TPaveStats.h>

#include "RatioData.h"
#include "file_io.h"

using namespace std;

// Structors
RatioData::RatioData() {
	divs = 0;
	num_ratios = -1;
}

RatioData::RatioData(int divisions) {
	divs = divisions;
	num_ratios = -1;
}

RatioData::~RatioData() {
	// Nothing
}


// Operators
RatioData& RatioData::operator += (RatioData& obj) {
	map<int, map<int, int>> new_ratio_data = obj.get_ratio_data();

	for(pair<int, map<int, int>> proton_per_event:new_ratio_data) {
		for(pair<int, int> proton_per_div:proton_per_event.second) {
			this->ratio_data[proton_per_event.first][proton_per_div.first] += proton_per_div.second;
		}
	}

	return(*this);
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

vector<double> RatioData::get_ratio_vec() {
	if(!ratio_vec_gen) {
		gen_ratio_vec();
	}

	return ratio_vec;
}

map<double, int> RatioData::get_ratio_hist() {
	if(!ratio_hist_gen) {
		gen_ratio_hist();
	}

	return ratio_hist;
}


int RatioData::get_num_ratios() {
	if(num_ratios < 0) {
		num_ratios = 0;
		if(!ratio_hist_gen) {
			gen_ratio_hist();
		}
		for(pair<double,int> ratio:ratio_hist) {
			num_ratios += ratio.second;
		}
	}

	return(num_ratios);
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
	proton_dist_gen = true;
}

void RatioData::gen_ratio_vec() {
	for(pair<int, map<int, int>> proton_per_event:ratio_data) {
		for(pair<int, int> proton_per_div:proton_per_event.second) {
			double ratio = ((double)proton_per_div.first)/proton_per_event.first;
			vector<double> ratio_batch(proton_per_div.second, ratio);
			ratio_vec.insert(ratio_vec.end(), ratio_batch.begin(), ratio_batch.end());
		}
	}
	ratio_vec_gen = true;
}

void RatioData::gen_ratio_hist() {
	for(pair<int, map<int, int>> proton_per_event:ratio_data) {
		for(pair<int, int> proton_per_div:proton_per_event.second) {
			double ratio = ((double)proton_per_div.first)/proton_per_event.first;
			ratio_hist[ratio] += proton_per_div.second;
		}
	}
	ratio_hist_gen = true;
}


// Plotters
void RatioData::canvas_2d_dist(string name, double p_clust) {
	TH2I *hist = ratios_map_to_hist(ratio_data, name);
	TCanvas *can = new TCanvas((name+"_Can").data());
	TF1 *avg_line = new TF1((name+"_avg").data(), ("x/"+to_string(divs)).data(), -0.5, 0.5+(--ratio_data.end())->first);
	TF1 *max_line = new TF1((name+"_max").data(), "x", -0.5, 0.5+(--ratio_data.end())->first);
	max_line->SetLineColor(4);
	can->SetLogz();
	hist->Draw("colz");
	max_line->Draw("same");
	avg_line->Draw("same");

	auto legend = new TLegend(0.1, 0.5, 0.4, 0.64);
	legend->AddEntry((name+"_avg").data(), ("Average Ratio (x/"+to_string(divs)+")").data(), "l");
	legend->AddEntry((name+"_max").data(), "Maximum Ratio (x)", "l");

	if(p_clust != -1) {
		int eff_bound = ceil(1.5 / p_clust); // Minimum num protons to get a grouping of 2 (therefore see any effect)
		TLine *eff_line = new TLine(eff_bound, 0, eff_bound, eff_bound);
		eff_line->SetLineColor(kRed);
		eff_line->Draw("same");
		legend->AddEntry(eff_line, "Effect Boundary", "l");
	}
	gPad->Update();
	TPaveStats *st = (TPaveStats*)hist->FindObject("stats");
	st->SetX1NDC(0.1);
	st->SetX2NDC(0.4);
	st->SetY1NDC(0.9);
	st->SetY2NDC(0.65);
	hist->GetXaxis()->SetTitle("Number of Protons in Event");
	hist->GetYaxis()->SetTitle("Number of Protons in Bin");
	legend->Draw();

	can->Write();

	delete avg_line;
	delete max_line;
	delete hist;
	delete can;
}


void RatioData::canvas_1d_dist(string name) {
	TH1D *hist = new TH1D(name.data(), name.data(), 23, -0.05, 1.1);
	for(pair<int, map<int, int>> event:ratio_data) {
		for(pair<int, int> bin:event.second) {
			for(int i=0; i<bin.second; i++) {
				hist->Fill(((double)bin.first) / event.first);
			}
		}
	}
	TCanvas *can = new TCanvas((name+"_Can").data());
	can->SetLogy();
	hist->Draw();

	can->Write();


	delete hist;
	delete can;
}


void RatioData::canvas_proton_dist(string name) {
	if(!proton_dist_gen) { gen_proton_dist(); }
	TH1D *hist = new TH1D(name.data(), name.data(), 51, -0.5, 50.5);
	for(pair<int, int> protons:proton_dist) {
		for(int i = 0; i < protons.second; i++) {
			hist->Fill(protons.first);
		}
	}
	TCanvas *can = new TCanvas((name+"_Can").data());
	hist->GetXaxis()->SetTitle("Number of Protons in Event");
	hist->Draw();

	can->Write();


	delete hist;
	delete can;
}
