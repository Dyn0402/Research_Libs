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

private:
	void calc_mean(bool err = true);
	void calc_standard_deviation(bool err = true);
	void calc_skewness(bool err = true);
	void calc_kurtosis(bool err = true);
	void calc_central_moment(int n);
	void calc_central_moment(vector<int> ns);

	vector<double> distribution;
	measure<double> mean, standard_deviation, skewness, kurtosis;
	map<int, double> central_moment;
	measure<bool> mean_calc, standard_deviation_calc, skewness_calc, kurtosis_calc;
	map<int, bool> central_moments_calc;
};


// Default constructor
Stats::Stats() {
	mean = 	standard_deviation = skewness = kurtosis = {0.0, 0.0};
	mean_calc = standard_deviation_calc = skewness_calc = kurtosis_calc = {false, false};
}


// Constructor with distribution passed.
Stats::Stats(vector<double> data) {
	distribution = data;
	mean = 	standard_deviation = skewness = kurtosis = {0.0, 0.0};
	mean_calc = standard_deviation_calc = skewness_calc = kurtosis_calc = {false, false};
}


// Return mean with error as measure struct
measure<double> Stats::get_mean() {
	calc_mean(true);
	return(mean);
}

// Return standard deviation with error as measure struct
measure<double> Stats::get_standard_deviation() {
	calc_standard_deviation(true);
	return(standard_deviation);
}

// Return skewness with error as measure struct
measure<double> Stats::get_skewness() {
	calc_skewness(true);
	return(skewness);
}

// Return kurtosis with error as measure struct
measure<double> Stats::get_kurtosis() {
	calc_kurtosis(true);
	return(kurtosis);
}


// Set distribution to passed data vector
void Stats::set_distribution(vector<double> data) {
	distribution = data;
}


// Calculate the mean of the distribution if not already calculated.
// Calculate corresponding uncertainty if err and not already calculated.
void Stats::calc_mean(bool err) {
	if(!mean_calc.val) {
		double sum = 0.0;
		for(double element:distribution) {
			sum += element;
		}
		mean.val = sum / distribution.size();
		mean_calc.val = true;
	}
	if(err && !mean_calc.err) {
		calc_standard_deviation(false);
		mean.err = standard_deviation.val / pow((double)distribution.size(), 0.5);
		mean_calc.err = true;
	}
}


// Calculate the standard deviation of the distribution if not already calculated.
// Calculate corresponding uncertainty if err and not already calculated.
void Stats::calc_standard_deviation(bool err) {
	if(!standard_deviation_calc.val) {
		double sum = 0.0;
		calc_central_moment(2);
		standard_deviation.val = pow(central_moment[2], 0.5);
		standard_deviation_calc.val = true;
	}
	if(err && !standard_deviation_calc.err) {
		calc_central_moment(4);
		standard_deviation.err = pow((central_moment[4] / pow(standard_deviation.val, 4) - 1) / (4 * distribution.size()), 0.5)  / pow((double)distribution.size(), 0.5);
		standard_deviation_calc.err = true;
	}
}


// Calculate the skewness of the distribution if not already calculated.
// Calculate corresponding uncertainty if err and not already calculated.
void Stats::calc_skewness(bool err) {
	if(!skewness_calc.val) {
		calc_central_moment({2,3});
		skewness.val = central_moment[3] / pow(central_moment[2], 1.5);
		kurtosis_calc.val = true;
	}
	if(err && !skewness_calc.err) {
		calc_central_moment({4,5,6});
		double m3 = central_moment[3] / pow(standard_deviation.val, 3);
		double m4 = central_moment[4] / pow(standard_deviation.val, 4);
		double m5 = central_moment[5] / pow(standard_deviation.val, 5);
		double m6 = central_moment[6] / pow(standard_deviation.val, 6);
		skewness.err = pow((9 - 6*m4 + pow(m3,2) * (35 + 9*m4) / 4 - 3*m3*m5 + m6) / distribution.size(), 0.5)  / pow((double)distribution.size(), 0.5);
		skewness_calc.err = true;
	}
}


// Calculate the kurtosis of the distribution if not already calculated.
// Calculate corresponding uncertainty if err and not already calculated.
void Stats::calc_kurtosis(bool err) {
	if(!kurtosis_calc.val) {
		calc_central_moment({2,4});
		kurtosis.val = central_moment[4] / pow(central_moment[2], 2) - 3;
		kurtosis_calc.val = true;
	}
	if(err && !kurtosis_calc.err) {
		calc_central_moment({3,5,6,8});
		double m3 = central_moment[3] / pow(standard_deviation.val, 3);
		double m4 = central_moment[4] / pow(standard_deviation.val, 4);
		double m5 = central_moment[5] / pow(standard_deviation.val, 5);
		double m6 = central_moment[6] / pow(standard_deviation.val, 6);
		double m8 = central_moment[8] / pow(standard_deviation.val, 8);
		kurtosis.err = pow((-pow(m4, 2) + 4*pow(m4, 3) + 16*pow(m3, 2) * (1 + m4) - 8*m3*m5 - 4*m4*m6 + m8) / distribution.size(), 0.5)  / pow((double)distribution.size(), 0.5);
		kurtosis_calc.err = true;
	}
}


// Calculate each central moment in ns.
void Stats::calc_central_moment(vector<int> ns) {
	for(int n:ns) {
		calc_central_moment(n);
	}
}


// Calculate nth order central moment if it has not already been calculated.
void Stats::calc_central_moment(int n) {
	if(!central_moments_calc[n]) {
		if(!mean_calc.val) { calc_mean(false); }
		double sum;
		for(double element:distribution) {
			sum += pow(element - mean.val, n);
		}
		central_moment[n] = sum / distribution.size();
		central_moments_calc[n] = true;
	}
}


#endif /* STATS_H_ */




