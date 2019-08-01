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

#include "RatioData.h"
#include "file_io.h"

using namespace std;

// Structors
RatioData::RatioData() {
	divs = 0;
}

RatioData::~RatioData() {
	// Nothing
}


// Getters
map<int, map<int, int>> RatioData::get_ratio_data() {
	return ratio_data;
}

map<int, int> RatioData::get_proton_dist() {
	if(!proton_dist_flag) {
		gen_proton_dist();
	}

	return proton_dist;
}


// Setters
void RatioData::set_divs(int divs_in) {
	divs = divs_in;
}


// Doers
void RatioData::read_data_from_file(string path) {
	ifstream in_file(path);
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
	}
}


void RatioData::gen_proton_dist() {
	for(pair<int, map<int, int>> proton_per_event:ratio_data) {
		for(pair<int, int> proton_per_div:proton_per_event.second) {
			proton_dist[proton_per_event.first] += proton_per_div.second;
		}
		proton_dist[proton_per_event.first] /= divs;
	}
}
