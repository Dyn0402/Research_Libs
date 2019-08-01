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
	// Nothing
}

RatioData::~RatioData() {
	// Nothing
}


// Getters
map<int, map<int, int>> RatioData::get_ratio_data() {
	return ratio_data;
}


// Setters



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
