/*
 * file_io.cpp
 *
 *  Created on: Jun 25, 2019
 *      Author: Dylan Neff
 */


#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <dirent.h>

#include "file_io.h"
#include "tree_reader.h"
#include "config.h"

using namespace std;



void write_tree_data(string job_id, tree_data data) {
	write_ratios(job_id, data.ratios);
	write_nprotons(job_id, data.good_protons);
}


void write_ratios(string job_id, map<int, map<int, map<int, map<int, int>>>> ratios) {
	for(pair<int, map<int, map<int, map<int, int>>>> divs:ratios) {
		for(pair<int, map<int, map<int, int>>> cents:divs.second) {
			string out_name = config::ratios_file_pre + config::file_name_delimeter;
			out_name += config::ratios_file_fields[0] + config::file_name_delimeter + to_string(divs.first);
			out_name += config::file_name_delimeter + config::ratios_file_fields[1] + config::file_name_delimeter + to_string(cents.first);
			out_name += config::file_name_delimeter + job_id + config::file_ext;
			ofstream out_file(out_name);
			for(pair<int, map<int, int>> nprotons:cents.second) {
				out_file << to_string(nprotons.first) << config::data_delimeters[0];
				for(pair<int, int> bin_protons:nprotons.second) {
					out_file << to_string(bin_protons.first) << config::data_delimeters[2] << to_string(bin_protons.second) << config::data_delimeters[1];
				}
				out_file << endl;
			}
			out_file.close();
		}
	}
}


void write_nprotons(string job_id, map<int, map<int, int>> good_protons) {
	for(pair<int, map<int,int>> cent:good_protons) {
		string out_name = config::nproton_file_pre + config::file_name_delimeter;
		out_name += config::nproton_file_fields[0] + config::file_name_delimeter + to_string(cent.first);
		out_name += config::file_name_delimeter + job_id + config::file_ext;
		ofstream out_file(out_name);
		for(pair<int,int> entry:cent.second) {
			out_file << to_string(entry.first) << config::data_delimeters[0] << to_string(entry.second) << endl;
		}
		out_file.close();
	}
}


map<int, int> read_nprotons(string path, int cent) {
	map<int, int> nprotons;
	DIR* files_dir = opendir(path.data());
	struct dirent* dp;
	while((dp=readdir(files_dir)) != NULL) {
		string file = dp->d_name;
		vector<string> fields = split_string_by_char(file, config::file_name_delimeter);
		if(fields[0] == config::nproton_file_pre) {
			if(stoi(fields[2]) == cent) {
				ifstream in_file(path+file);
				string line;
				if(in_file.is_open()) {
					while(getline(in_file,line)) {
						vector<string> elements = split_string_by_char(line, config::data_delimeters[0]);
						nprotons[stoi(elements[0])] += stoi(elements[1]);
					}
				}
			}
		}
	}
	return(nprotons);
}



map<int, map<int, int>> read_ratios(string path, int div, int cent) {
	map<int, map<int, int>> ratios;
	DIR* files_dir = opendir(path.data());
	struct dirent* dp;
	while((dp=readdir(files_dir)) != NULL) {
		string file = dp->d_name;
		vector<string> fields = split_string_by_char(file, config::file_name_delimeter);
		if(fields[0] == config::ratios_file_pre) {
			if(stoi(fields[2]) == div) {
				if(stoi(fields[4]) == cent) {
					ifstream in_file(path+file);
					string line;
					if(in_file.is_open()) {
						while(getline(in_file,line)) {
							vector<string> line_elements = split_string_by_char(line, config::data_delimeters[0]);
							vector<string> data_elements = split_string_by_char(line_elements[1], config::data_delimeters[1]);
							for(string element:data_elements) {
								if(element.size() > 1) {
									vector<string> datum = split_string_by_char(element, config::data_delimeters[2]);
									ratios[stoi(line_elements[0])][stoi(datum[0])] += stoi(datum[1]);
								}
							}
						}
					}
				}
			}
		}
	}
	return(ratios);
}


vector<string> split_string_by_char(string str, char del) {
	vector<string> split;
	stringstream stream(str);
	string hold;
	while(getline(stream, hold, del)) {
		split.push_back(hold);
	}

	return(split);
}
