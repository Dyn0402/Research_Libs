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

#include <TFile.h>

using namespace std;


class RatioData {
private:
	map<int, map<int, int>> ratio_data;
	map<int, int> proton_dist;
	int divs;

	bool ratio_data_gen = false;
	bool proton_dist_gen = false;

	string nproton_file_pre = "nprotons";
	vector<string> nproton_file_fields = {"centrality"};
	char file_name_delimeter = '_';
	vector<char> data_delimeters = {'\t', ' ', ':'};
	string ratios_file_pre = "ratios";
	vector<string> ratios_file_fields = {"divisions", "centrality"};
	string file_ext = ".txt";

	// Doers
	void gen_proton_dist();

public:
	// Structors
	RatioData(int divisions);
	~RatioData();

	// Getters
	map<int, map<int, int>> get_ratio_data();
	map<int, int> get_proton_dist();
	int get_divs();

	// Setters
	void set_ratio_data(map<int, map<int, int>> ratio_data_in);
	void set_divs(int divs_in);

	// Doers
	void read_data_from_file(string file_path);
	void read_ratios_from_dir(string dir_path, int div, int cent = 99);
	void write_ratios(string job_id, string dir_path, int div, int cent = 99);

	// Plotters
	void canvas_2d_dist(TFile *out_file, string name, double p_clust = -99);
};


#endif /* RATIODATA_H_ */
