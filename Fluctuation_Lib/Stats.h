/*
 * Stats.h
 *
 *  Created on: Jul 15, 2019
 *      Author: Dylan Neff
 *
 *  Based on John D Cook class on johndcook.com
 */


#include <string>
#include <vector>
#include <map>
#include <cmath>

using namespace std;


#ifndef STATS_H_
#define STATS_H_

//Represents a measurment with a vlaue and a corresponding uncertainty.
struct measure {
	double val, err;
};


class Stats {
public:
	Stats();
	Stats(vector<double> data);

	void set_distribution(vector<double> data);

	measure get_skewness();
	measure get_standard_deviation();
	measure get_mean();
	measure get_kurtosis();
private:
	void calc_mean();
	void calc_standard_deviation();
	void calc_skewness();
	void calc_kurtosis();
	void calc_central_moment(int n);

	vector<double> distribution;
	measure mean, standard_deviation, skewness, kurtosis;
	map<int, double> central_moments;
	bool mean_calc, standard_deviation_calc, skewness_calc, kurtosis_calc;
	map<int, bool> central_moments_calc;


};


Stats::Stats() {
	mean = {0.0, 0.0};
	standard_deviation = {0.0, 0.0};
	skewness = {0.0, 0.0};
	kurtosis = {0.0, 0.0};
	mean_calc = standard_deviation_calc = skewness_calc = kurtosis_calc = false;
}


Stats::Stats(vector<double> data) {
	distribution = data;
	mean = {0.0, 0.0};
	standard_deviation = {0.0, 0.0};
	skewness = {0.0, 0.0};
	kurtosis = {0.0, 0.0};
	mean_calc = standard_deviation_calc = skewness_calc = kurtosis_calc = false;
}


void Stats::calc_central_moment(int n) {
	double mean = get_mean();
	double moment = 0;
	for(double x:distribution) {
		moment += pow((x - mean), n);
	}

	return(moment / distribution.size());
}


void Stats::calc_mean() {
	if(true) {
		double sum = 0.0;
		for(double element:distribution) {
			sum += element;
		}
		mean.val = sum / distribution.size();

	}
}

#endif /* STATS_H_ */




