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
template <typename T>
struct measure {
	T val, err;
};

// Equations to propagate error with measure type. Should probably build into its own class?
measure<double> divide(measure<double> numerator, measure<double> denominator);


class Stats {
public:
	// Structors
	Stats();
	Stats(vector<double> data);
	Stats(map<double, int> data);

	// Setters
	void set_distribution(vector<double> data);
	void set_distribution(map<double, int> data);

	// Getters
	Measure get_skewness();
	Measure get_standard_deviation();
	Measure get_mean();
	Measure get_kurtosis();
	Measure get_cumulant(int order);
	int get_dist_num();

private:
	// Doers
	void calc_mean(bool err = true);
	void calc_standard_deviation(bool err = true);
	void calc_skewness(bool err = true);
	void calc_kurtosis(bool err = true);
	void calc_cumulant(int n, bool err = true);
	void calc_central_moment(int n);
	void calc_central_moment(vector<int> ns);
	void calc_dist_num();

	vector<double> distribution;
	map<double, int> distribution_hist;

	int dist_num;

	string dist_type; // "vec" for vector<double>, "hist" for map<double, int>

	measure<double> mean, standard_deviation, skewness, kurtosis;
	map<int, measure<double>> cumulant;
	map<int, double> central_moment;

	measure<bool> mean_calc, standard_deviation_calc, skewness_calc, kurtosis_calc;
	map<int, measure<bool>> cumulant_calc;
	map<int, bool> central_moments_calc;
};

#endif /* STATS_H_ */




