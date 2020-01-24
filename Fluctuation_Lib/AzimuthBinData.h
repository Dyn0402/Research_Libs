/*
 * AzimuthBinData.h
 *
 *  Created on: Jul 31, 2019
 *      Author: Dylan Neff
 */

#ifndef AZIMUTHBINDATA_H_
#define AZIMUTHBINDATA_H_

#include <vector>
#include <string>
#include <map>

#include <TFile.h>

using namespace std;


class AzimuthBinData {
public:
	// Structors
	AzimuthBinData();
	AzimuthBinData(int divisions);
	~AzimuthBinData();

	// Getters
	map<int, map<int, int>> get_bin_data();
	map<int, int> get_proton_dist();
	vector<double> get_ratio_vec();
	map<double, int> get_ratio_hist();
	map<double, int> get_diff_hist();
	int get_divs();
	int get_num_ratios();

	// Setters
	void set_ratio_data(map<int, map<int, int>> ratio_data_in);
	void set_divs(int divs_in);

	// Doers
	void read_data_from_file(string file_path);
	void read_data_from_dir(string dir_path, int div, int cent = 99);
	void write_data(string job_id, string dir_path, int div, int cent = 99);

	// Plotters
	void canvas_2d_dist(string name, double p_clust = -99);
	void canvas_ratio_dist(string name);
	void canvas_proton_dist(string name);

	// Operators
	AzimuthBinData& operator += (AzimuthBinData& obj);

private:
	map<int, map<int, int>> bin_data;  // Base data storage. Most efficient.
	map<int, int> proton_dist;
	vector<double> ratio_vec;
	map<double, int> ratio_hist;
	map<double, int> diff_hist;
	int divs;
	int num_ratios;

	bool bin_data_gen = false;
	bool proton_dist_gen = false;
	bool ratio_vec_gen = false;
	bool ratio_hist_gen = false;
	bool diff_hist_gen = false;

	string nproton_file_pre = "nprotons";
	vector<string> nproton_file_fields = {"centrality"};
	char file_name_delimeter = '_';
	vector<char> data_delimeters = {'\t', ' ', ':'};
	string ratios_file_pre = "ratios";
	vector<string> ratios_file_fields = {"divisions", "centrality"};
	string file_ext = ".txt";

	// Doers
	void gen_proton_dist();
	void gen_ratio_vec();
	void gen_ratio_hist();
	void gen_diff_hist();
};


#endif /* AZIMUTHBINDATA_H_ */
