/*
 * file_io.cpp
 *
 *  Created on: Jun 25, 2019
 *      Author: Dylan Neff
 */


#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <sys/stat.h>

#ifdef _WIN32
	#include "dirent_win.h"
#else
	#include <dirent.h>
#endif

#include "file_io.h"

#include "config_lib.h"
#include "ratio_methods.h"

using namespace std;



bool check_path(string path) {
	struct stat info;

	if( stat(path.data(), &info) !=0 ) {
		return false;
	} else {
		return true;
	}
}

void write_tree_data(string job_id, map<int, map<int, map<int, map<int, int>>>> data, string path) {
	struct stat info;

	if( stat(path.data(), &info) !=0 ) {
		cout << "Can't access path: " << path << " Skipping following read/write." << endl;
	} else {
		write_ratios(job_id, data, path);
	}
}


void write_ratios(string job_id, map<int, map<int, map<int, map<int, int>>>> ratios, string path) {
	for(pair<int, map<int, map<int, map<int, int>>>> divs:ratios) {
		for(pair<int, map<int, map<int, int>>> cents:divs.second) {
			string out_name = path + io::ratios_file_pre + io::file_name_delimeter;
			out_name += io::ratios_file_fields[0] + io::file_name_delimeter + to_string(divs.first);
			out_name += io::file_name_delimeter + io::ratios_file_fields[1] + io::file_name_delimeter + to_string(cents.first);
			out_name += io::file_name_delimeter + job_id + io::file_ext;
			ofstream out_file(out_name);
			for(pair<int, map<int, int>> nprotons:cents.second) {
				out_file << to_string(nprotons.first) << io::data_delimeters[0];
				for(pair<int, int> bin_protons:nprotons.second) {
					out_file << to_string(bin_protons.first) << io::data_delimeters[2] << to_string(bin_protons.second) << io::data_delimeters[1];
				}
				out_file << endl;
			}
			out_file.close();
		}
	}
}


void write_nprotons(string job_id, map<int, map<int, int>> good_protons, string path) {
	for(pair<int, map<int,int>> cent:good_protons) {
		string out_name = path + io::nproton_file_pre + io::file_name_delimeter;
		out_name += io::nproton_file_fields[0] + io::file_name_delimeter + to_string(cent.first);
		out_name += io::file_name_delimeter + job_id + io::file_ext;
		ofstream out_file(out_name);
		for(pair<int,int> entry:cent.second) {
			out_file << to_string(entry.first) << io::data_delimeters[0] << to_string(entry.second) << endl;
		}
		out_file.close();
	}
}


map<int, int> read_nprotons(string path, int cent) {
	map<int, int> nprotons;
	struct stat info;

	if( stat(path.data(), &info) !=0 ) {
	cout << "Can't access path: " << path << " Skipping following read/write." << endl;
	} else {
		DIR* files_dir = opendir(path.data());
		struct dirent* dp;
		while((dp=readdir(files_dir)) != NULL) {
			string file = dp->d_name;
			vector<string> fields = split_string_by_char(file, io::file_name_delimeter);
			if(fields[0] == io::nproton_file_pre) {
				if(stoi(fields[2]) == cent) {
					ifstream in_file(path+file);
					string line;
					if(in_file.is_open()) {
						while(getline(in_file,line)) {
							vector<string> elements = split_string_by_char(line, io::data_delimeters[0]);
							nprotons[stoi(elements[0])] += stoi(elements[1]);
						}
					}
				}
			}
		}
	}
	return(nprotons);
}



map<int, map<int, int>> read_ratios(string path, int div, int cent) {
	map<int, map<int, int>> ratios;
	struct stat info;

	if( stat(path.data(), &info) !=0 ) {
		cout << "Can't access path: " << path << " Skipping following read/write." << endl;
	} else {
		DIR* files_dir = opendir(path.data());
		struct dirent* dp;
		while((dp=readdir(files_dir)) != NULL) {
			string file = dp->d_name;
			vector<string> fields = split_string_by_char(file, io::file_name_delimeter);
			if(fields[0] == io::ratios_file_pre) {
				if(stoi(fields[2]) == div) {
					if(stoi(fields[4]) == cent) {
						ifstream in_file(path+file);
						string line;
						if(in_file.is_open()) {
							while(getline(in_file,line)) {
								vector<string> line_elements = split_string_by_char(line, io::data_delimeters[0]);
								vector<string> data_elements = split_string_by_char(line_elements[1], io::data_delimeters[1]);
								for(string element:data_elements) {
									if(element.size() > 1) {
										vector<string> datum = split_string_by_char(element, io::data_delimeters[2]);
										ratios[stoi(line_elements[0])][stoi(datum[0])] += stoi(datum[1]);
									}
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


//Return name (not path) of all files in dir_path with extension ext.
vector<string> get_files_in_dir(string dir_path, string ext, string out) {
	vector<string> files;
	struct stat info;

	if( stat(dir_path.data(), &info) !=0 ) {
		cout << "Can't access path: " << dir_path << " Skipping following read/write." << endl;
	} else {
		DIR* files_dir = opendir(dir_path.data());
		struct dirent* dp;
		while((dp=readdir(files_dir)) != NULL) {
			string file = dp->d_name;
			vector<string> fields = split_string_by_char(file, '.');
			if(fields.back() == ext) {
				files.push_back(file);
			}
		}
		if(out == "path") {
			for(unsigned i=0; i<files.size(); i++) {
				files[i] = dir_path + files[i];
			}
		}
	}

	return(files);
}


// Check if sub is in main.
bool in_string(string main, string sub) {
	if (main.find(sub) != string::npos) {
		return(true);
	}
	return(false);
}


// Check if any subs are in main. If all is true, check if all subs are in main.
bool in_string(string main, vector<string> subs, bool all) {

	for(string sub:subs) {
		if(in_string(main, sub)) {
			if(!all) {
				return(true);
			}
		} else if(all) {
			return(false);
		}
	}

	if(all) {
		return(true);
	} else {
		return(false);
	}
}


// Emulation of Python split function. Split string into vector of strings on delim.
vector<string> split(string main, char delim) {
	if(main.size() == 0) { return {}; }

	vector<string> split_strings {""};
	for(char x:main) {
		if(x == delim) {
			if(split_strings.back() != "") {
				split_strings.push_back("");
			}
		} else {
			split_strings.back() += x;
		}
	}
	return(split_strings);
}


// Emulation of Python split function. Split string into vector of strings on delim.
//vector<string> split(string main, string delim) {
//	if(delim.size() <= 0) { return main; }
//
//	vector<string> split_strings {""};
//	int main_index = 0;
//	while(main_index < (int)main.size()) {
//		int sub_index = 0;
//		string temp = "";
//		while(main[main_index] == delim[sub_index]) {
//			//
//		}
//		split_strings.back().insert(split_strings.back().begin(), temp.begin(), temp.end());
//	}
//	for (char x:main) {
//		if(x == delim[0]) {
//			if(split_strings.back() != "") {
//				split_strings.push_back("");
//			}
//		} else {
//			split_strings.back() += x;
//		}
//	}
//	return(split_strings);
//}


// Return list of all centralities of specific div that exist in dir_path
vector<int> get_centrals(string dir_path, int div) {
	vector<int> centrals;
	struct stat info;

	if( stat(dir_path.data(), &info) !=0 ) {
		cout << "Can't access path: " << dir_path << " Skipping following read/write." << endl;
	} else {
		DIR* files_dir = opendir(dir_path.data());
		struct dirent* dp;
		while((dp=readdir(files_dir)) != NULL) {
			string file = dp->d_name;
			vector<string> fields = split_string_by_char(file, io::file_name_delimeter);
			if(fields[0] == io::ratios_file_pre) {
				if(stoi(fields[2]) == div) {
					int cent = stoi(fields[4]);
					if(count(centrals.begin(), centrals.end(), cent) == 0) {
						centrals.push_back(cent);
					}
				}
			}
		}
		sort(centrals.begin(), centrals.end());
	}

	return(centrals);
}


// Get file name from path string
string get_name_from_path(string path) {
	string file_name = split_string_by_char(path, '/').back();
	return(file_name);
}
