/*
 * ratio_methods.h
 *
 *  Created on: Jun 17, 2019
 *      Author: Dylan Neff
 */

#ifndef RATIO_METHODS_H_
#define RATIO_METHODS_H_

#include <map>
#include <vector>

using namespace std;

struct tree_data {
	map<int, map<int, map<int, map<int, int>>>> ratios; //ratios[divisions][centrality][num protons in event][num protons in bin]
	map<int, map<int, int>> good_protons; //good_protons[centrality][num protons]
};

vector<int> get_Rs(vector<double> angles, int divisions);
double rotate_angle(double angle, double rotate);
vector<double> rotate_angles(vector<double> angles, double rotate);

double get_cumulant(vector<double> data, int n);
double get_central_moment(vector<double> data, int n);
double get_raw_moment(vector<double> data, int n);

vector<int> nproton_map_to_vec(map<int, int> nproton);
vector<double> ratios_map_to_vec(map<int, map<int, int>> ratios);

#endif /* RATIO_METHODS_H_ */
