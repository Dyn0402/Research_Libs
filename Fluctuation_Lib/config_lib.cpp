/*
 * lconfig.cpp
 *
 *  Created on: Jun 27, 2019
 *      Author: Dylan Neff
 */


#include <string>
#include <vector>
#include <map>

using namespace std;


namespace io {
	//File io
	string nproton_file_pre = "nprotons";
	vector<string> nproton_file_fields = {"centrality"};
	char file_name_delimeter = '_';
	vector<char> data_delimeters = {'\t', ' ', ':'};
	string ratios_file_pre = "ratios";
	vector<string> ratios_file_fields = {"divisions", "centrality"};
	string file_ext = ".txt";
}
