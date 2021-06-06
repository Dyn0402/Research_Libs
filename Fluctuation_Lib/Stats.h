/*
 * Stats.h
 *
 *  Created on: Jul 15, 2019
 *      Author: Dylan Neff
 *
 *  Based on John D Cook class on johndcook.com
 */


#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cmath>

#include "Measure.h"

using namespace std;


#ifndef STATS_H_
#define STATS_H_


// Represents a measurement with a value and a corresponding uncertainty.
// Only need this for bools, could probably transition to Measure class.
template <typename T>
struct measure {
	T val, err;
};


class Stats {
public:
	// Structors
	Stats();
	Stats(vector<double> data);
	Stats(map<double, int> data);
	Stats(map<int, int> data);

	// Setters
	void set_distribution(vector<double> data);
	void set_distribution(map<double, int> data);
	void set_distribution(map<int, int> data);
	void set_nan_check(bool nan_check);

	// Getters
	Measure get_mean();
	Measure get_standard_deviation();
	Measure get_skewness();
	Measure get_skew_sd();
	Measure get_kurtosis();
	Measure get_non_excess_kurtosis();
	Measure get_kurt_var();
	Measure get_cumulant(int order);
	Measure get_raw_moment(int order);
	Measure get_central_moment(int order);
	long long int get_dist_num();
	bool get_nan_check();

private:
	// Doers
	void calc_mean(bool err = true);
	void calc_standard_deviation(bool err = true);
	void calc_skewness(bool err = true);
	void calc_skew_sd(bool err = true);
	void calc_kurtosis(bool err = true);
	void calc_kurt_var(bool err = true);
	void calc_cumulant(int n, bool err = true);
	void calc_central_moment(int n);
	void calc_central_moment(vector<int> ns);
	void calc_raw_moment(int n);
	void calc_raw_moment(vector<int> ns);
	void calc_dist_num();

	vector<double> distribution;
	map<double, int> distribution_hist;

	bool nan_check;
	long long int dist_num;

	string dist_type; // "vec" for vector<double>, "hist" for map<double, int>

	measure<double> mean, standard_deviation, skewness, skew_sd, kurtosis, kurt_var;
	map<int, measure<double>> cumulant;
	map<int, double> central_moment;
	map<int, double> raw_moment;

	measure<bool> mean_calc, standard_deviation_calc, skewness_calc, skew_sd_calc, kurtosis_calc, kurt_var_calc;
	map<int, measure<bool>> cumulant_calc;
	map<int, bool> central_moment_calc;
	map<int, bool> raw_moment_calc;
};

#endif /* STATS_H_ */




