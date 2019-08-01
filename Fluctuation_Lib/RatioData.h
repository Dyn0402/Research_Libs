/*
 * RatioData.h
 *
 *  Created on: Jul 31, 2019
 *      Author: Dylan Neff
 */

#ifndef RATIODATA_H_
#define RATIODATA_H_

#include <vector>
#include <string>
#include <map>

using namespace std;


class RatioData {
private:
	map<int, map<int, int>> ratio_data;

	string nproton_file_pre = "nprotons";
	vector<string> nproton_file_fields = {"centrality"};
	char file_name_delimeter = '_';
	vector<char> data_delimeters = {'\t', ' ', ':'};
	string ratios_file_pre = "ratios";
	vector<string> ratios_file_fields = {"divisions", "centrality"};
	string file_ext = ".txt";

public:
	// Structors
	RatioData();
	~RatioData();

	// Getters
	map<int, map<int, int>> get_ratio_data();
	map<int, int> get_proton_dist;

	// Setters

	// Doers
	void read_data_from_file(string path);
};


#endif /* RATIODATA_H_ */
