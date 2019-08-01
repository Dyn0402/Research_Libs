/*
 * Stats.cpp
 *
 *  Created on: Jul 16, 2019
 *      Author: Dylan Neff
 */


#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cmath>

#include "Stats.h"

using namespace std;


// Structors
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



// Getters
// Return mean with error as measure struct
Measure Stats::get_mean() {
	calc_mean(true);
	Measure mean_out(mean.val, mean.err);
	return(mean_out);
}

// Return standard deviation with error as measure struct
Measure Stats::get_standard_deviation() {
	calc_standard_deviation(true);
	Measure standard_deviation_out(standard_deviation.val, standard_deviation.err);
	return(standard_deviation_out);
}

// Return skewness with error as measure struct
Measure Stats::get_skewness() {
	calc_skewness(true);
	Measure skewness_out(skewness.val, skewness.err);
	return(skewness_out);
}

// Return kurtosis with error as measure struct
Measure Stats::get_kurtosis() {
	calc_kurtosis(true);
	Measure kurtosis_out(kurtosis.val, kurtosis.err);
	return(kurtosis_out);
}

// Return cumulant of order n with error as measure struct
Measure Stats::get_cumulant(int order) {
	calc_cumulant(order);
	Measure cumulant_out(cumulant[order].val, cumulant[order].err);
	return(cumulant_out);
}


// Setters
// Set distribution to passed data vector
void Stats::set_distribution(vector<double> data) {
	distribution = data;
}


// Doers
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
		calc_central_moment(2);
		standard_deviation.val = pow(central_moment[2], 0.5);
		standard_deviation_calc.val = true;
	}
	if(err && !standard_deviation_calc.err) {
		calc_central_moment(4);
		standard_deviation.err = pow((central_moment[4] / pow(standard_deviation.val, 4) - 1) / (4 * distribution.size()), 0.5);
		standard_deviation_calc.err = true;
	}
}


// Calculate the skewness of the distribution if not already calculated.
// Calculate corresponding uncertainty if err and not already calculated.
void Stats::calc_skewness(bool err) {
	if(!skewness_calc.val) {
		calc_central_moment({2,3});
		skewness.val = central_moment[3] / pow(central_moment[2], 1.5);
		skewness_calc.val = true;
	}
	if(err && !skewness_calc.err) {
		calc_central_moment({4,5,6});
		double m3 = central_moment[3] / pow(standard_deviation.val, 3);
		double m4 = central_moment[4] / pow(standard_deviation.val, 4);
		double m5 = central_moment[5] / pow(standard_deviation.val, 5);
		double m6 = central_moment[6] / pow(standard_deviation.val, 6);
		skewness.err = pow((9 - 6*m4 + pow(m3,2) * (35 + 9*m4) / 4 - 3*m3*m5 + m6) / distribution.size(), 0.5);
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
		kurtosis.err = pow((-pow(m4, 2) + 4*pow(m4, 3) + 16*pow(m3, 2) * (1 + m4) - 8*m3*m5 - 4*m4*m6 + m8) / distribution.size(), 0.5);
		kurtosis_calc.err = true;
	}
}


// Calculate the cumulant of order n of the distribution if not already calculated.
// Calculate corresponding uncertainty if err and not already calculated.
void Stats::calc_cumulant(int order, bool err) {
	map<int, double> &mu = central_moment; //Alias central_moment as mu to shorten calculations.

	if(!cumulant_calc[order].val) {
		if(order == 1) {
			calc_mean(false);
			cumulant[order].val = mean.val;
			cumulant_calc[order].val = true;
		}
		else if(order == 2) {
			calc_central_moment(2);
			cumulant[order].val = mu[2];
			cumulant_calc[order].val = true;
		}
		else if(order == 3) {
			calc_central_moment(3);
			cumulant[order].val = mu[3];
			cumulant_calc[order].val = true;
		}
		else if(order == 4) {
			calc_central_moment({2,4});
			cumulant[order].val = mu[4] - 3 * pow(mu[2], 2);
			cumulant_calc[order].val = true;
		}
		else {
			cout << "Cumulant of order " << order << " not implemented." << endl;
		}
	}

	if(err && !cumulant_calc[order].err) {
		if(order == 1) {
			calc_central_moment(2);
			cumulant[order].err = pow(mu[2] / distribution.size(), 0.5);
			cumulant_calc[order].err = true;
		}
		else if(order == 2) {
			calc_central_moment({2,4});
			cumulant[order].err = pow((mu[4] - pow(mu[2], 2)) / distribution.size(), 0.5);
			cumulant_calc[order].err = true;
		}
		else if(order == 3) {
			calc_central_moment({2,3,4,6});
			cumulant[order].err = pow(( mu[6] - pow(mu[3],2) + 9*pow(mu[2],3) - 6*mu[2]*mu[4] ) / distribution.size(), 0.5);
			cumulant_calc[order].err = true;
		}
		else if(order == 4) {
			calc_central_moment({2,3,4,5,6,8});
			cumulant[order].err = pow(( mu[8] - 12*mu[6]*mu[2] - 8*mu[5]*mu[3] - pow(mu[4], 2) + 48*mu[4]*pow(mu[2],2) + 64*pow(mu[3],2)*mu[2] - 36*pow(mu[2],4) ) / distribution.size(), 0.5);
			cumulant_calc[order].err = true;
		}
		else {
			cout << "Cumulant error of order " << order << " not implemented." << endl;
		}
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
		double sum = 0.0;
		for(double element:distribution) {
			sum += pow(element - mean.val, n);
		}
		central_moment[n] = sum / distribution.size();
		central_moments_calc[n] = true;
	}
}
