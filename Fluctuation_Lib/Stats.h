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

using namespace std;


#ifndef STATS_H_
#define STATS_H_


// Represents a measurement with a value and a corresponding uncertainty.
template <typename T>
struct measure {
	T val, err;
};


class Stats {
public:
	Stats();
	Stats(vector<double> data);

	void set_distribution(vector<double> data);

	measure<double> get_skewness();
	measure<double> get_standard_deviation();
	measure<double> get_mean();
	measure<double> get_kurtosis();
	measure<double> get_cumulant(int order);

private:
	void calc_mean(bool err = true);
	void calc_standard_deviation(bool err = true);
	void calc_skewness(bool err = true);
	void calc_kurtosis(bool err = true);
	void calc_cumulant(int n, bool err = true);
	void calc_central_moment(int n);
	void calc_central_moment(vector<int> ns);

	vector<double> distribution;

	measure<double> mean, standard_deviation, skewness, kurtosis;
	map<int, measure<double>> cumulant;
	map<int, double> central_moment;

	measure<bool> mean_calc, standard_deviation_calc, skewness_calc, kurtosis_calc;
	map<int, measure<bool>> cumulant_calc;
	map<int, bool> central_moments_calc;
};

#endif /* STATS_H_ */




