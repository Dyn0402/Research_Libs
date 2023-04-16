/*
 * ratio_methods.h
 *
 *  Created on: Jun 17, 2019
 *      Author: Dylan Neff
 */

#ifndef RATIO_METHODS_H_
#define RATIO_METHODS_H_

#include <map>
#include <tuple>
#include <vector>

#include "Measure.h"

#include <TH2.h>
#include <TRandom3.h>

using namespace std;

struct tree_data {
	map<int, map<int, map<int, map<int, int>>>> ratios; //ratios[divisions][centrality][num protons in event][num protons in bin]
	map<int, map<int, int>> good_protons; //good_protons[centrality][num protons]
};

vector<int> get_Rs(const vector<double>& angles, int divisions);
vector<int> get_Rs(const vector<double>& angles, double bin_width, TRandom3 *r, int bin_num = 1);
vector<int> get_resamples3(vector<double> angles, double bin_width, int samples);
vector<int> get_resamples4(vector<double> angles, double bin_width, int samples, TRandom3* r);
vector<int> get_resamples4(vector<double> angles, double bin_width, int samples, vector<double> r);
vector<int> get_resamples5(vector<double> angles, double bin_width, int samples, TRandom3* r);
vector<int> get_resamples4_test(vector<double> angles, double bin_width, int samples, vector<double> bin_lows);
vector<int> get_resamples5_test(vector<double> angles, double bin_width, int samples, vector<double> bin_lows);
vector<int> get_resamples2(vector<double> angles, double bin_width, int samples);
map<int, int> get_resamples1(vector<double> angles, double bin_width, int samples);
double rotate_angle(double angle, double rotate);
vector<double> rotate_angles(const vector<double>& angles, double rotate);

tuple<double, double> get_cumulant(vector<double> data, int n);
double get_central_moment(vector<double> data, int n);
double get_raw_moment(vector<double> data, int n);

vector<int> nproton_map_to_vec(map<int, int> nproton);
vector<double> ratios_map_to_vec(map<int, map<int, int>> ratios);
TH2I* ratios_map_to_hist(map<int, map<int, int>> ratios, string name);
void sum_tree_data(tree_data *base, tree_data add);

vector<double> get_two_particle_correlation(vector<double> &angles);

double sample_sd(vector<double> data);
double sample_sd(vector<Measure> data);
Measure median(vector<Measure> data);

#endif /* RATIO_METHODS_H_ */
