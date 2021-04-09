/*
 * file_io.h
 *
 *  Created on: Jun 25, 2019
 *      Author: Dylan Neff
 */

#ifndef FILE_IO_H_
#define FILE_IO_H_

#include <vector>
#include <map>

#include "ratio_methods.h"

using namespace std;


bool check_path(string path);

void write_tree_data(string job_id, map<int, map<int, map<int, map<int, int>>>> data, string path="");

void write_ratios(string job_id, map<int, map<int, map<int, map<int, int>>>> ratios, string path="");
void write_nprotons(string job_id, map<int, map<int, int>> good_protons, string path="");

map<int, int> read_nprotons(string path, int cent);
map<int, map<int, int>> read_ratios(string path, int div, int cent);

vector<string> split_string_by_char(string str, char del);
vector<string> get_files_in_dir(string dir_path, string ext, string out="name", bool dirs=false);
string get_name_from_path(string path);
bool in_string(string main, string sub);
bool in_string(string main, vector<string> subs, bool all = false);
vector<string> split(string main, char delim = ' ');
vector<string> split(string main, string delim = " ");
string strip_flag(string main, string flag = " ", bool keep_flag = false, bool tail = false);
vector<string> get_flag_trail(string main, string flag, string delim);
float str_num_dec(string main, int dec_pos = 1);

vector<int> get_centrals(string dir_path, int div);

#endif /* FILE_IO_H_ */
