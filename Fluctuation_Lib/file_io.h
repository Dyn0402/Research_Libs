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


void write_tree_data(string job_id, tree_data);

void write_ratios(string job_id, map<int, map<int, map<int, map<int, int>>>> ratios);
void write_nprotons(string job_id, map<int, map<int, int>> good_protons);

map<int, int> read_nprotons(string path, int cent);
map<int, map<int, int>> read_ratios(string path, int div, int cent);

vector<string> split_string_by_char(string str, char del);

#endif /* FILE_IO_H_ */
