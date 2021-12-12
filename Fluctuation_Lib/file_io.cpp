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
static const std::string platform = "win";
#else
#include <dirent.h>
static const std::string platform = "lin";
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

void write_tree_data(string job_id, vector<vector<vector<vector<long>>>> &data, vector<int> divs, int cent_min, int particle_min, string path) {
	struct stat info;

	if( stat(path.data(), &info) !=0 ) {
		cout << "Can't access path: " << path << " Skipping following read/write." << endl;
	} else {
		write_dists(job_id, data, divs, cent_min, particle_min, path);
	}
}


void write_tree_data_bootstrap(string job_id, vector<vector<vector<vector<long>>>> &data, vector<vector<vector<vector<vector<long>>>>> &data_bs, vector<int> divs, int cent_min, int particle_min, string path) {
	struct stat info;

	if (stat(path.data(), &info) != 0) {
		cout << "Can't access path: " << path << " Skipping following read/write." << endl;
	}
	else {
		write_dists_bootstrap(job_id, data, data_bs, divs, cent_min, particle_min, path);
	}
}


void write_dists(string job_id, vector<vector<vector<vector<long>>>> &data, vector<int> divs, int cent_min, int particle_min, string path) {
	for (unsigned div_i=0; div_i < data.size(); div_i++) {
		for (unsigned cent_i=0; cent_i < data[div_i].size(); cent_i++) {
			bool write = false;
			for (unsigned particle_bin=0; particle_bin < data[div_i][cent_i].size(); particle_bin++) {
				for (long count : data[div_i][cent_i][particle_bin]) {
					if (count != 0) {
						write = true;
						break;
					}
				}
			}
			if (!write) { continue; }  // Only write files with some nonzero data.
			string out_name = path + io::ratios_file_pre + io::file_name_delimeter;
			out_name += io::ratios_file_fields[0] + io::file_name_delimeter + to_string(divs[div_i]);
			out_name += io::file_name_delimeter + io::ratios_file_fields[1] + io::file_name_delimeter + to_string((int)cent_i + cent_min);
			out_name += io::file_name_delimeter + job_id + io::file_ext;
			ofstream out_file(out_name);
			for (unsigned particle_bin=0; particle_bin < data[div_i][cent_i].size(); particle_bin++) {
				write = false;
				for (long count : data[div_i][cent_i][particle_bin]) {
					if (count != 0) {
						write = true;
						break;
					}
				}
				if (write) {
					out_file << to_string(particle_bin + particle_min) << io::data_delimeters[0];
					for (unsigned num_protons=0; num_protons < data[div_i][cent_i][particle_bin].size(); num_protons++) {
						if (data[div_i][cent_i][particle_bin][num_protons] != 0) {
							out_file << to_string(num_protons) << io::data_delimeters[2] << data[div_i][cent_i][particle_bin][num_protons] << io::data_delimeters[1];
						}
					}
					out_file << endl;
				}
			}
			out_file.close();
		}
	}
}


void write_dists_bootstrap(string job_id, vector<vector<vector<vector<long>>>> &data, vector<vector<vector<vector<vector<long>>>>> &data_bs, vector<int> divs, int cent_min, int particle_min, string path) {
	for (unsigned div_i=0; div_i < data.size(); div_i++) {
		for (unsigned cent_i=0; cent_i < data[div_i].size(); cent_i++) {
			bool write = false;
			for (unsigned particle_bin=0; particle_bin < data[div_i][cent_i].size(); particle_bin++) {
				for (long count : data[div_i][cent_i][particle_bin]) {
					if (count != 0) {
						write = true;
						break;
					}
				}
			}
			if (!write) { continue; }  // Only write files with some nonzero data.
			string out_name = path + io::ratios_file_pre + io::file_name_delimeter;
			out_name += io::ratios_file_fields[0] + io::file_name_delimeter + to_string(divs[div_i]);
			out_name += io::file_name_delimeter + io::ratios_file_fields[1] + io::file_name_delimeter + to_string((int)cent_i + cent_min);
			out_name += io::file_name_delimeter + job_id + io::file_ext;
			ofstream out_file(out_name);
			for (unsigned particle_bin=0; particle_bin < data[div_i][cent_i].size(); particle_bin++) {
				write = false;
				for (long count : data[div_i][cent_i][particle_bin]) {
					if (count != 0) {
						write = true;
						break;
					}
				}
				if (write) {
					out_file << to_string(particle_bin + particle_min) << io::data_delimeters[0];
					for (unsigned num_protons=0; num_protons < data[div_i][cent_i][particle_bin].size(); num_protons++) {
						if (data[div_i][cent_i][particle_bin][num_protons] != 0) {
							out_file << to_string(num_protons) << io::data_delimeters[2] << data[div_i][cent_i][particle_bin][num_protons] << io::data_delimeters[1];
						}
					}
					out_file << endl;
				}
			}

			for (unsigned bs_i=0; bs_i < data_bs[div_i][cent_i].size(); bs_i++) {
				out_file << endl << "bootstrap #" << bs_i<< endl;

				for (unsigned particle_bin=0; particle_bin < data[div_i][cent_i].size(); particle_bin++) {
					bool write = false;
					for (long count : data_bs[div_i][cent_i][bs_i][particle_bin]) {
						if (count != 0) {
							write = true;
							break;
						}
					}
					if (write) {
						out_file << to_string(particle_bin + particle_min) << io::data_delimeters[0];
						for (unsigned num_protons=0; num_protons < data_bs[div_i][cent_i][bs_i][particle_bin].size(); num_protons++) {
							if (data_bs[div_i][cent_i][bs_i][particle_bin][num_protons] != 0) {
								out_file << to_string(num_protons) << io::data_delimeters[2] << data_bs[div_i][cent_i][bs_i][particle_bin][num_protons] << io::data_delimeters[1];
							}
						}
						out_file << endl;
					}
				}
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


// Return name (not path) of all files in dir_path with extension ext.
// If dirs, get directories instead of files and ignore ext.
vector<string> get_files_in_dir(string dir_path, string ext, string out, bool dirs) {
	vector<string> files;
	struct stat info;

	if( stat(dir_path.data(), &info) !=0 ) {
		cout << "Can't access path: " << dir_path << " Skipping following read/write." << endl;
	} else {
		DIR* files_dir = opendir(dir_path.data());
		struct dirent* dp;
		while((dp=readdir(files_dir)) != NULL) {
			if(dirs) {
				if(dp->d_type == DT_DIR) {
					files.push_back(dp->d_name);
				}
			} else {
				string file = dp->d_name;
				vector<string> fields = split_string_by_char(file, '.');
				if(fields.back() == ext) {
					files.push_back(file);
				}
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
vector<string> split(string main, string delim) {
	if(delim.size() <= 0) { return {}; }
	size_t index = main.find(delim);

	vector<string> split_strings {};
	do {
		split_strings.push_back(main.substr(0, index));
		main = main.substr(index + delim.size());
		index = main.find(delim);
	}
	while(index != string::npos);

	if(main.size() > 0) { split_strings.push_back(main); }

	return(split_strings);
}


// Find last instance of flag and return preceding string.
// If tail, find first instance and return following string.
string strip_flag(string main, string flag, bool keep_flag, bool tail) {
	size_t pos = main.rfind(flag);
	if(tail) {
		if(pos == string::npos) {
			return main;
		} else {
			if(!keep_flag) { pos += flag.size(); }
			return(main.substr(pos, main.size() - pos));
		}
	} else {
		if(pos == string::npos) {
			return main;
		} else {
			if(keep_flag) { pos += flag.size(); }
			return(main.substr(0, pos));
		}
	}
}


// Find all occurances of find in string main and replace with replace. Return altered main.
string find_replace(string main, string find, string replace) {
	string::size_type n = 0;
	while ((n = main.find(find, n)) != string::npos) {
		main.replace(n, find.size(), replace);
		n += replace.size();
	}

	return main;
}


// Make directory corresponding to path. If recreate is true, delete directory before making it again.
void mkdir(string path, bool recreate) {
	if (platform == "win") { path = find_replace(path, "/", "\\"); }

	struct stat info;
	int find_stat = stat(path.data(), &info);

	if (info.st_mode & S_IFDIR && find_stat == 0) {  // Directory exists
		if (recreate) {
			if (platform == "lin") { int i = system(("rm -r " + path).data()); }
			else if (platform == "win") { int i = system(("rmdir /Q /S " + path).data()); }
		}
		else { return; }
	}  // Else directory does not exist or path is not a directory
	
	if (system(("mkdir " + path).data())) { cout << "Could not create output directory " << path << endl; }
}


// Check if directory corresponding to path exists.
bool check_dir(string path) {
	if (platform == "win") { path = find_replace(path, "/", "\\"); }

	struct stat info;
	int find_stat = stat(path.data(), &info);

	if ((info.st_mode & S_IFDIR) && find_stat == 0) {  // Directory exists
		return true;
	} else {
		return false;
	}
}


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


// Find flag in string separated by delims, return string trailing flag before next delim
vector<string> get_flag_trail(string main, string flag, string delim) {
	vector<string> split_string = split(main, delim);

	vector<string> trails {};
	for(string sub_str:split_string) {
		size_t index = sub_str.find(flag);
		if(index != string::npos) {
			trails.push_back(sub_str.substr(index + flag.size()));
		}
	}
	return(trails);
}


// Given number as string, convert to float with decimal place given via dec_pos, default 1 (100 --> 1.00)
float str_num_dec(string main, int dec_pos) {
	int num_int;
	stringstream(main) >> num_int;
	int len = (main[0] == '-') ? main.size() - 1 : main.size();
	float num_float = num_int / pow(10, len - dec_pos);

	return(num_float);
}
