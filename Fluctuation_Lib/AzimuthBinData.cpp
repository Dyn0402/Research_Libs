/*
 * AzimuthBinData.cpp
 *
 *  Created on: Jul 31, 2019
 *      Author: Dylan Neff
 */


#include "AzimuthBinData.h"

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <map>

#ifdef _WIN32
#include "dirent_win.h"
#else
#include <dirent.h>
#endif

#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <TF1.h>
#include <TLine.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TLegend.h>
#include <TPaveStats.h>

#include "file_io.h"

using namespace std;

// Structors
AzimuthBinData::AzimuthBinData() {
	divs = 120;
	num_ratios = -1;
	diff_divisor = 1;
}

AzimuthBinData::AzimuthBinData(int divisions) {
	divs = divisions;
	num_ratios = -1;
	diff_divisor = 1;
}

AzimuthBinData::~AzimuthBinData() {
	// Nothing
}


// Operators
AzimuthBinData& AzimuthBinData::operator += (AzimuthBinData& obj) {
	map<int, map<int, int>> new_ratio_data = obj.get_bin_data();

	for(auto &proton_per_event:new_ratio_data) {
		for(auto &proton_per_div:proton_per_event.second) {
			this->bin_data[proton_per_event.first][proton_per_div.first] += proton_per_div.second;
		}
	}

	if(this->bin_data.size() > 0) {
		this->bin_data_gen = true;
	}

	return *this;
}


// Getters
map<int, map<int, int>> AzimuthBinData::get_bin_data() {
	if(!bin_data_gen) {
		cout << "No ratio data generated. Returning empty map." << endl;
	}
	return bin_data;
}


map<int, int> AzimuthBinData::get_bin_data_slice(int total_protons) {
	if(!bin_data_gen) {
		cout << "No ratio data generated. Returning empty map." << endl;
	}
	return bin_data[total_protons];
}


map<int, int> AzimuthBinData::get_proton_dist() {
	if(!proton_dist_gen) {
		gen_proton_dist();
	}

	return proton_dist;
}

double AzimuthBinData::get_proton_dist_mean() {
	if(!proton_dist_gen) {
		gen_proton_dist();
	}

	int sum = 0;
	int num = 0;
	for(auto &nprotons:proton_dist) {
		num += nprotons.second;
		sum += nprotons.first * nprotons.second;
	}

	return((double)sum/num);
}

vector<double> AzimuthBinData::get_ratio_vec() {
	if(!ratio_vec_gen) {
		gen_ratio_vec();
	}

	return ratio_vec;
}

map<double, int> AzimuthBinData::get_ratio_hist() {
	if(!ratio_hist_gen) {
		gen_ratio_hist();
	}

	return ratio_hist;
}

map<double, int> AzimuthBinData::get_diff_hist(string div_flag) {
	if(!diff_hist_gen) { gen_diff_hist(); }

	if(div_flag == "yes") {
		map<double, int> div_diff_hist;
		for(auto &diff:diff_hist) {
			div_diff_hist[diff.first/diff_divisor] += diff.second;
		}
		return div_diff_hist;
	} else { return diff_hist; }
}


map<int, map<double, int>> AzimuthBinData::get_diff_slice_hist(string div_flag) {
	if(!diff_slice_hist_gen) { gen_diff_slice_hist(); }

	if(div_flag == "yes") {
		map<int, map<double, int>> div_diff_slice_hist;
		for(auto &event:diff_slice_hist) {
			if(diff_slice_divisor[event.first] > 0) {
				for(auto &diff:event.second) {
					div_diff_slice_hist[event.first][diff.first/diff_slice_divisor[event.first]] += diff.second;
				}
			} else { cout << event.first << " total particles, divisor " << diff_slice_divisor[event.first] << endl; }
		}
		return div_diff_slice_hist;
	} else { return diff_slice_hist; }
}

map<double, int> AzimuthBinData::get_diff_slice_proj(string div_flag) {
	if(!diff_slice_hist_gen) { gen_diff_slice_hist(); }

	map<int, map<double, int>> div_diff_slice_hist;
	if(div_flag == "yes") {
		for(auto &event:diff_slice_hist) {
			for(auto &diff:event.second) {
				if(diff_slice_divisor[event.first] > 0) {
					div_diff_slice_hist[event.first][diff.first/diff_slice_divisor[event.first]] += diff.second;
				} //else { cout << event.first << " | " << diff.first << endl; }
			}
		}
	} else {
		div_diff_slice_hist = diff_slice_hist;
	}

	map<double, int> div_diff_slice_proj;
	for(auto &event:div_diff_slice_hist) {
		for(auto &diff:event.second) {
//			if(isinf(diff.first)) { cout << event.first << " | " << diff.first << endl; }
			div_diff_slice_proj[diff.first] += diff.second;
		}
	}

	return div_diff_slice_proj;
}

long AzimuthBinData::get_num_bins() {
	if (num_ratios < 0) {
		num_ratios = 0;
		if (!ratio_hist_gen) {
			gen_ratio_hist();
		}
		for (auto &ratio : ratio_hist) {
			num_ratios += ratio.second;
		}
	}

	return num_ratios;
}

double AzimuthBinData::get_diff_divisor() {
	return diff_divisor;
}


// Setters
void AzimuthBinData::set_ratio_data(map<int, map<int, int>> ratio_data_in) {
	bin_data = ratio_data_in;
	bin_data_gen = true;
}

void AzimuthBinData::set_divs(int divs_in) {
	divs = divs_in;
}

void AzimuthBinData::set_diff_divisor(double divisor) {
	diff_divisor = divisor;
}

void AzimuthBinData::set_diff_slice_divisor(int total_particles, double divisor) {
	if(divisor > 0) {
		diff_slice_divisor[total_particles] = divisor;
	} else {
		cout << "Invalid divisor of: " << divisor << " for " << total_particles << " total particles" << endl;
	}
}

// Doers
void AzimuthBinData::read_data_from_file(string file_path) {
	ifstream in_file(file_path);
	string line;
	if(in_file.is_open()) {
		while(getline(in_file,line)) {
			vector<string> line_elements = split_string_by_char(line, data_delimeters[0]);
			vector<string> data_elements = split_string_by_char(line_elements[1], data_delimeters[1]);
			for(string &element:data_elements) {
				if(element.size() > 1) {
					vector<string> datum = split_string_by_char(element, data_delimeters[2]);
					bin_data[stoi(line_elements[0])][stoi(datum[0])] += stoi(datum[1]);
				}
			}
		}
	bin_data_gen = true;
	}
}


void AzimuthBinData::read_data_from_dir(string dir_path, int div, int cent) {
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
							for(string &element:data_elements) {
								if(element.size() > 1) {
									vector<string> datum = split_string_by_char(element, data_delimeters[2]);
									bin_data[stoi(line_elements[0])][stoi(datum[0])] += stoi(datum[1]);
								}
							}
						}
						bin_data_gen = true;
					}
				}
			}
		}
	}
}


void AzimuthBinData::write_data(string unique_suffix, string dir_path, int div, int cent) {
	string out_name = dir_path + ratios_file_pre + file_name_delimeter;
	out_name += ratios_file_fields[0] + file_name_delimeter + to_string(div);
	out_name += file_name_delimeter + ratios_file_fields[1] + file_name_delimeter + to_string(cent);
	out_name += file_name_delimeter + unique_suffix + file_ext;
	ofstream out_file(out_name);
	for(auto &nprotons:bin_data) {
		out_file << to_string(nprotons.first) << data_delimeters[0];
		for(auto &bin_protons:nprotons.second) {
			out_file << to_string(bin_protons.first) << data_delimeters[2] << to_string(bin_protons.second) << data_delimeters[1];
		}
		out_file << endl;
	}
	out_file.close();
}


void AzimuthBinData::print_data() {
	for (pair<int, map<int, int>> event : bin_data) {
		cout << event.first << "\t" << flush;
		for (pair<int, int> bin : event.second) {
			cout << bin.first << ":" << bin.second << " " << flush;
		}
		cout << endl;
	}
}

void AzimuthBinData::gen_proton_dist() {
	for(auto &event:bin_data) {
		for(auto &bin:event.second) {
			proton_dist[event.first] += bin.second;
		}
//		proton_dist[event.first] /= divs;  I think this was to normalize dist to number of events.
	}
	proton_dist_gen = true;
}

void AzimuthBinData::gen_ratio_vec() {
	for(auto &event:bin_data) {
		for(auto &bin:event.second) {
			double ratio = (double)bin.first / event.first;
			vector<double> ratio_batch(bin.second, ratio);
			ratio_vec.insert(ratio_vec.end(), ratio_batch.begin(), ratio_batch.end());
		}
	}
	ratio_vec_gen = true;
}

void AzimuthBinData::gen_ratio_hist() {
	for(auto &event:bin_data) {
		for(auto &bin:event.second) {
			double ratio = (double)bin.first / event.first;
			ratio_hist[ratio] += bin.second;
		}
	}
	ratio_hist_gen = true;
}

void AzimuthBinData::gen_diff_hist() {
	for(auto &event:bin_data) {
		for(auto &bin:event.second) {
			double diff = bin.first - (double)event.first * divs / 360;
			diff_hist[diff] += bin.second;
		}
	}
	diff_hist_gen = true;
}

void AzimuthBinData::gen_diff_slice_hist() {
	for(auto &event:bin_data) {
		for(auto &bin:event.second) {
			double diff = bin.first - (double)event.first * divs / 360;
			diff_slice_hist[event.first][diff] += bin.second;
		}
	}
	diff_slice_hist_gen = true;
}


// Plotters
void AzimuthBinData::canvas_2d_dist(string name, double p_clust) {
	if(bin_data.size() <= 0) { return; }  // Bad workaround, fix.
	TH2I *hist = ratios_map_to_hist(bin_data, name);
	TCanvas *can = new TCanvas((name+"_Can").data());
	TF1 *avg_line = new TF1((name+"_avg").data(), ("x/360.0*"+to_string(divs)).data(), -0.5, 0.5+(--bin_data.end())->first);
	TF1 *max_line = new TF1((name+"_max").data(), "x", -0.5, 0.5+(--bin_data.end())->first);
	max_line->SetLineColor(4);
	gStyle->SetOptStat("KSiouRMen");  //221112211
	can->SetLogz();
	hist->Draw("colz");
	max_line->Draw("same");
	avg_line->Draw("same");

	auto legend = new TLegend(0.1, 0.45, 0.4, 0.59);
	legend->AddEntry((name+"_avg").data(), ("Average Ratio (x*360/"+to_string(divs)+")").data(), "l");
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
	st->SetY2NDC(0.6);
	hist->GetXaxis()->SetTitle("Number of Protons in Event");
	hist->GetYaxis()->SetTitle("Number of Protons in Bin");
	legend->Draw();

	can->Write();

	delete avg_line;
	delete max_line;
	delete hist;
	delete can;
}


void AzimuthBinData::canvas_ratio_dist(string name) {
	TH1D *hist = new TH1D(name.data(), name.data(), 23, -0.05, 1.1);
	for(auto &event:bin_data) {
		for(auto &bin:event.second) {
			hist->Fill((double)bin.first / event.first, bin.second);
		}
	}
	TCanvas *can = new TCanvas((name+"_Can").data());
	can->SetLogy();
	gStyle->SetOptStat("KSiouRMen");  //221112211
	hist->Draw("HIST");
	gPad->Update();

	can->Write();


	delete hist;
	delete can;
}


void AzimuthBinData::canvas_ratio_dist_slow(string name, int bin_mult) {
	TH1D* hist = new TH1D(name.data(), name.data(), 23 * bin_mult, -0.05, 1.1);
	for (auto& event : bin_data) {
		for (auto& bin : event.second) {
			for (int i = 0; i < bin.second; i++) {
				hist->Fill((double)bin.first / event.first);
			}
		}
	}
	TCanvas* can = new TCanvas((name + "_Can").data());
	can->SetLogy();
	gStyle->SetOptStat("KSiouRMen");  //221112211
	hist->Draw("HIST");
	gPad->Update();

	can->Write();


	delete hist;
	delete can;
}


void AzimuthBinData::canvas_diff_dist(string name, string div_flag) {
	if(!diff_hist_gen) { gen_diff_hist(); }
	double divisor = 1;
//	if(div_flag == "yes") { divisor = diff_divisor; }
	TH1D *hist = new TH1D(name.data(), name.data(), 1001, -20, 20);  // Guess at binning/range, fix.
	for(auto &event:bin_data) {
		if(div_flag == "yes") { divisor = diff_slice_divisor[event.first]; }
		else { divisor = 1; }
		for(auto &bin:event.second) {
			hist->Fill((bin.first - (double)event.first * divs / 360) / divisor, bin.second);
		}
	}
	TCanvas *can = new TCanvas((name+"_Can").data());
	can->SetLogy();
	gStyle->SetOptStat("KSiouRMen");  //221112211
	hist->Draw("HIST");
	gPad->Update();

	can->Write();


	delete hist;
	delete can;
}


void AzimuthBinData::canvas_proton_dist(string name) {
	if(!proton_dist_gen) { gen_proton_dist(); }
	TH1D *hist = new TH1D(name.data(), name.data(), 61, -0.5, 60.5);
	for(auto &protons:proton_dist) {
		hist->Fill(protons.first, protons.second);
	}
	TCanvas *can = new TCanvas((name+"_Can").data());
	hist->GetXaxis()->SetTitle("Number of Protons in Event");
	gStyle->SetOptStat("KSiouRMen");  //221112211
	hist->Draw("HIST");
	gPad->Update();

	can->Write();


	delete hist;
	delete can;
}
