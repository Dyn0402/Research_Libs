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
	mean = standard_deviation = skewness = skew_sd = kurtosis = kurt_var = {0.0, 0.0};
	mean_calc = standard_deviation_calc = skewness_calc = skew_sd_calc = kurtosis_calc = kurt_var_calc = {false, false};
	dist_type = "vec";
	dist_num = 0;
	nan_check = true;
}

// Constructor with distribution passed.
Stats::Stats(vector<double> data) {
	distribution = data;
	mean = standard_deviation = skewness = skew_sd = kurtosis = kurt_var = {0.0, 0.0};
	mean_calc = standard_deviation_calc = skewness_calc = skew_sd_calc = kurtosis_calc = kurt_var_calc = {false, false};
	dist_type = "vec";
	calc_dist_num();
	nan_check = true;
}

// Constructor with distribution passed as a double histogram.
Stats::Stats(map<double, int> data) {
	distribution_hist = data;
	mean = standard_deviation = skewness = skew_sd = kurtosis = kurt_var = {0.0, 0.0};
	mean_calc = standard_deviation_calc = skewness_calc = skew_sd_calc = kurtosis_calc = kurt_var_calc = {false, false};
	dist_type = "hist";
	calc_dist_num();
	nan_check = true;
}

// Constructor with distribution passed as an int histogram.
Stats::Stats(map<int, int> data) {
	for(pair<int, int> datum:data) { distribution_hist[(double)datum.first] = datum.second; }
	mean = standard_deviation = skewness = skew_sd = kurtosis = kurt_var = {0.0, 0.0};
	mean_calc = standard_deviation_calc = skewness_calc = skew_sd_calc = kurtosis_calc = kurt_var_calc = {false, false};
	dist_type = "hist";
	calc_dist_num();
	nan_check = true;
}

// Getters
// Return mean with error as Measure object
Measure Stats::get_mean() {
	calc_mean(true);
	Measure mean_out(mean.val, mean.err);
	if(nan_check) {
		if(std::isnan(mean_out.get_val()) || std::isnan(mean_out.get_err())) {
			cout << "WARNING: Nan in mean value or error" << endl;
		}
	}
	return(mean_out);
}

// Return standard deviation with error as Measure object
Measure Stats::get_standard_deviation() {
	calc_standard_deviation(true);
	Measure standard_deviation_out(standard_deviation.val, standard_deviation.err);
	if(nan_check) {
		if(std::isnan(standard_deviation_out.get_val()) || std::isnan(standard_deviation_out.get_err())) {
			cout << "WARNING: Nan in standard deviation value or error" << endl;
		}
	}
	return(standard_deviation_out);
}

// Return skewness with error as Measure object
Measure Stats::get_skewness() {
	calc_skewness(true);
	Measure skewness_out(skewness.val, skewness.err);
	if(nan_check) {
		if(std::isnan(skewness_out.get_val()) || std::isnan(skewness_out.get_err())) {
			cout << "WARNING: Nan in skewness value or error" << endl;
		}
	}
	return(skewness_out);
}

// Return skewness*sd with error as Measure object
Measure Stats::get_skew_sd() {
	calc_skew_sd(true);
	Measure skew_sd_out(skew_sd.val, skew_sd.err);
	if(nan_check) {
		if(std::isnan(skew_sd_out.get_val()) || std::isnan(skew_sd_out.get_err())) {
			cout << "WARNING: Nan in skewness value or error" << endl;
		}
	}
	return(skew_sd_out);
}

// Return excess kurtosis with error as Measure object
Measure Stats::get_kurtosis() {
	calc_kurtosis(true);
	Measure kurtosis_out(kurtosis.val, kurtosis.err);
	if(nan_check) {
		if(std::isnan(kurtosis_out.get_val()) || std::isnan(kurtosis_out.get_err())) {
			cout << "WARNING: Nan in kurtosis value or error" << endl;
		}
	}
	return(kurtosis_out);
}

// Return non-excess kurtosis with error as Measure object
// 0.1
Measure Stats::get_non_excess_kurtosis() {
	calc_kurtosis(true);
	Measure kurtosis_out(kurtosis.val, kurtosis.err);
	kurtosis_out = kurtosis_out + 3;
	if(nan_check) {
		if(std::isnan(kurtosis_out.get_val()) || std::isnan(kurtosis_out.get_err())) {
			cout << "WARNING: Nan in kurtosis value or error" << endl;
		}
	}
	return(kurtosis_out);
}

// Return excess kurtosis with error as Measure object
Measure Stats::get_kurt_var() {
	calc_kurt_var(true);
	Measure kurt_var_out(kurt_var.val, kurt_var.err);
	if(nan_check) {
		if(std::isnan(kurt_var_out.get_val()) || std::isnan(kurt_var_out.get_err())) {
			cout << "WARNING: Nan in kurtosis value or error" << endl;
		}
	}
	return(kurt_var_out);
}


// Return cumulant of order n with error as Measure object
Measure Stats::get_cumulant(int order) {
	calc_cumulant(order);
	Measure cumulant_out(cumulant[order].val, cumulant[order].err);
	if(nan_check) {
		if(std::isnan(cumulant_out.get_val()) || std::isnan(cumulant_out.get_err())) {
			cout << "WARNING: Nan in cumulant order " << order << " value or error" << endl;
		}
	}
	return(cumulant_out);
}

// Return central moment of order n with error as Measure object
Measure Stats::get_central_moment(int order) {
	calc_central_moment(order);
	Measure moment_out(central_moment[order], 0.0); // No error, need to add later.
	if(nan_check) {
		if(std::isnan(moment_out.get_val()) || std::isnan(moment_out.get_err())) {
			cout << "WARNING: Nan in cumulant order " << order << " value or error" << endl;
		}
	}
	return(moment_out);
}

// Return central moment of order n with error as Measure object
Measure Stats::get_raw_moment(int order) {
	calc_raw_moment(order);
	Measure moment_out(raw_moment[order], 0.0); // No error, need to add later.
	if(nan_check) {
		if(std::isnan(moment_out.get_val()) || std::isnan(moment_out.get_err())) {
			cout << "WARNING: Nan in cumulant order " << order << " value or error" << endl;
		}
	}
	return(moment_out);
}

// Return number of entries in distribution
long long int Stats::get_dist_num() {
	return dist_num;
}

// Return boolean nan_check indicating whether nan warning is printed
bool Stats::get_nan_check() {
	return nan_check;
}


// Setters
// Set distribution to passed data vector
void Stats::set_distribution(vector<double> data) {
	distribution = data;
	dist_type = "vec";
	calc_dist_num();
}

// Set distribution to passed data map
void Stats::set_distribution(map<double, int> data) {
	distribution_hist = data;
	dist_type = "hist";
	calc_dist_num();
}

// Set distribution to passed data map
void Stats::set_distribution(map<int, int> data) {
	for(pair<int, int> datum:data) { distribution_hist[(double)datum.first] = datum.second; }
	dist_type = "hist";
	calc_dist_num();
}

// Set nan_check indicating whether nan warning is printed
void Stats::set_nan_check(bool nan_check) {
	this->nan_check = nan_check;
}


// Doers
// Calculate the mean of the distribution if not already calculated.
// Calculate corresponding uncertainty if err and not already calculated.
void Stats::calc_mean(bool err) {
	if(!mean_calc.val) {
		double sum = 0.0;
		if(dist_type == "vec") {
			for (const double& element:distribution) {
				sum += element;
			}
			mean.val = sum / distribution.size();
		} else if(dist_type == "hist") {
			for (const pair<double, int>& entry:distribution_hist) {
				sum += entry.second * entry.first;
			}
			mean.val = sum / dist_num;
		} else { cout << "dist_type not recognized." << endl; }
		mean_calc.val = true;
	}
	if(err && !mean_calc.err) {
		calc_standard_deviation(false);
		mean.err = standard_deviation.val / pow(dist_num, 0.5);
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
		double m4 = central_moment[4] / pow(central_moment[2], 2);
		standard_deviation.err = pow((m4 - 1) * central_moment[2] / (dist_num * 4), 0.5);
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
		if(!standard_deviation_calc.val) { calc_standard_deviation(false); }
		double m3 = central_moment[3] / pow(standard_deviation.val, 3);
		double m4 = central_moment[4] / pow(standard_deviation.val, 4);
		double m5 = central_moment[5] / pow(standard_deviation.val, 5);
		double m6 = central_moment[6] / pow(standard_deviation.val, 6);
		skewness.err = pow((9 - 6*m4 + pow(m3,2) * (35 + 9*m4) / 4 - 3*m3*m5 + m6) / dist_num, 0.5);
		skewness_calc.err = true;
	}
}


// Calculate the skewness*standard deviation of the distribution if not already calculated.
// Calculate corresponding uncertainty if err and not already calculated.
void Stats::calc_skew_sd(bool err) {
	if(!skew_sd_calc.val) {
		calc_central_moment({2,3});
		skew_sd.val = central_moment[3] / central_moment[2];
		skew_sd_calc.val = true;
	}
	if(err && !skew_sd_calc.err) {
		calc_central_moment({2,3,4,5,6});
		if(!standard_deviation_calc.val) { calc_standard_deviation(false); }
		double m3 = central_moment[3] / pow(standard_deviation.val, 3);
		double m4 = central_moment[4] / pow(standard_deviation.val, 4);
		double m5 = central_moment[5] / pow(standard_deviation.val, 5);
		double m6 = central_moment[6] / pow(standard_deviation.val, 6);
		skew_sd.err = pow((9 - 6*m4 + pow(m3,2) * (6 + m4) - 2*m3*m5 + m6) * central_moment[2] / dist_num, 0.5);
		skew_sd_calc.err = true;
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
		calc_central_moment({3,4,5,6,8});
		if(!standard_deviation_calc.val) { calc_standard_deviation(false); }
		double m3 = central_moment[3] / pow(standard_deviation.val, 3);
		double m4 = central_moment[4] / pow(standard_deviation.val, 4);
		double m5 = central_moment[5] / pow(standard_deviation.val, 5);
		double m6 = central_moment[6] / pow(standard_deviation.val, 6);
		double m8 = central_moment[8] / pow(standard_deviation.val, 8);
		kurtosis.err = pow((-pow(m4, 2) + 4*pow(m4, 3) + 16*pow(m3, 2) * (1 + m4) - 8*m3*m5 - 4*m4*m6 + m8) / dist_num, 0.5);
		kurtosis_calc.err = true;
	}
}


// Calculate the kurtosis*variance of the distribution if not already calculated.
// Calculate corresponding uncertainty if err and not already calculated.
void Stats::calc_kurt_var(bool err) {
	if(!kurt_var_calc.val) {
		calc_central_moment({2,4});
		kurt_var.val = central_moment[4] / central_moment[2] - 3 * central_moment[2];
		kurt_var_calc.val = true;
	}
	if(err && !kurt_var_calc.err) {
		calc_central_moment({2,3,4,5,6,8});
		if(!standard_deviation_calc.val) { calc_standard_deviation(false); }
		double m3 = central_moment[3] / pow(standard_deviation.val, 3);
		double m4 = central_moment[4] / pow(standard_deviation.val, 4);
		double m5 = central_moment[5] / pow(standard_deviation.val, 5);
		double m6 = central_moment[6] / pow(standard_deviation.val, 6);
		double m8 = central_moment[8] / pow(standard_deviation.val, 8);
		kurt_var.err = pow((-9 + 6*pow(m4,2) + pow(m4, 3) + 8*pow(m3,2)*(5+m4) - 8*m3*m5 + m4*(9-2*m6) - 6*m6 + m8) * pow(central_moment[2], 2) / dist_num, 0.5);
		kurt_var_calc.err = true;
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
		else if(order == 5) {  // Not cross-checked! arxiv:1809.08892
			calc_central_moment({2,3,5});
			cumulant[order].val = mu[5] - 10 * mu[3] * mu[2];
			cumulant_calc[order].val = true;
		}
		else if(order == 6) {  // Not cross-checked! arxiv:1809.08892
			calc_central_moment({2,3,4,6});
			cumulant[order].val = mu[6] - 15 * mu[4] *mu[2] - 10 * pow(mu[3], 2) + 30 * pow(mu[2], 3);
			cumulant_calc[order].val = true;
		}
		else {
			cout << "Cumulant of order " << order << " not implemented." << endl;
		}
	}

	if(err && !cumulant_calc[order].err) {
		if(order == 1) {
			calc_central_moment(2);
			cumulant[order].err = pow(mu[2] / dist_num, 0.5);
			cumulant_calc[order].err = true;
		}
		else if(order == 2) {
			calc_central_moment({2,4});
			cumulant[order].err = pow((mu[4] - pow(mu[2], 2)) / dist_num, 0.5);
			cumulant_calc[order].err = true;
		}
		else if(order == 3) {
			calc_central_moment({2,3,4,6});
			cumulant[order].err = pow(( mu[6] - pow(mu[3],2) + 9*pow(mu[2],3) - 6*mu[2]*mu[4] ) / dist_num, 0.5);
			cumulant_calc[order].err = true;
		}
		else if(order == 4) {
			calc_central_moment({2,3,4,5,6,8});
			cumulant[order].err = pow(( mu[8] - 12*mu[6]*mu[2] - 8*mu[5]*mu[3] - pow(mu[4], 2) + 48*mu[4]*pow(mu[2],2) + 64*pow(mu[3],2)*mu[2] - 36*pow(mu[2],4) ) / dist_num, 0.5);
			cumulant_calc[order].err = true;
		}
		else if(order == 5) {  // Not cross-checked! arxiv:1809.08892
			calc_central_moment({2,3,4,5,6,7,8,10});
			cumulant[order].err = pow(( mu[10] - pow(mu[5],2) - 10*mu[4]*mu[6] + 900*pow(mu[2],5) - 20*mu[3]*mu[7] - 20*mu[8]*mu[2] + 125*mu[2]*pow(mu[4],2) + 200*mu[4]*pow(mu[3],2) - 1000*pow(mu[3]*mu[2],2) + 160*mu[6]*pow(mu[2],2) - 900*mu[4]*pow(mu[2],3) + 240*mu[2]*mu[3]*mu[5] ) / dist_num, 0.5);
			cumulant_calc[order].err = true;
		}
		else if(order == 6) {  // Not cross-checked! arxiv:1809.08892
			calc_central_moment({2,3,4,5,6,7,8,9,10,12});
			cumulant[order].err = pow(( -30*mu[4]*mu[8] + 510*mu[4]*mu[2]*mu[6] + 1020*mu[4]*mu[3]*mu[5] + 405*mu[8]*pow(mu[2],2) - 2880*mu[6]*pow(mu[2],3) - 9720*mu[3]*mu[5]*pow(mu[2],2) - 30*mu[2]*mu[10] + 840*mu[2]*mu[3]*mu[7] + 216*mu[2]*pow(mu[5],2) - 40*mu[3]*mu[9] + 440*mu[6]*pow(mu[3],2) - 3600*pow(mu[2]*mu[4],2) - 9600*mu[2]*mu[4]*pow(mu[3],2) + 13500*mu[4]*pow(mu[2],4) + 39600*pow(mu[2],3)*pow(mu[3],2) + mu[12] - pow(mu[6],2) - 12*mu[5]*mu[7] + 225*pow(mu[4],3) - 8100*pow(mu[2],6) - 400*pow(mu[3],4) ) / dist_num, 0.5);
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
	if(!central_moment_calc[n]) {
		if(!mean_calc.val) { calc_mean(false); }
		double sum = 0.0;
		if(dist_type == "vec") {
			for (const double& element:distribution) {
				sum += pow(element - mean.val, n);
			}
		} else if(dist_type == "hist") {
			for (const pair<double, int>& entry:distribution_hist) {
				sum += entry.second * pow(entry.first - mean.val, n);
			}
		} else { cout << "dist_type not recognized." << endl; }

		central_moment[n] = sum / dist_num;
		central_moment_calc[n] = true;
	}
}


// Calculate each raw moment in ns.
void Stats::calc_raw_moment(vector<int> ns) {
	vector<int> calc_ns;
	for(int n:ns) {
		if(!raw_moment_calc[n]) { calc_ns.push_back(n); }
	}

	map<int, double> sums;
	if(dist_type == "vec") {
		for (const double& element:distribution) {
			for(int n:calc_ns) {
				sums[n] += pow(element, n);
			}
		}
	} else if(dist_type == "hist") {
		for (const pair<double, int>& entry:distribution_hist) {
			for(int n:calc_ns) {
				sums[n] += entry.second * pow(entry.first, n);
			}
		}
	} else { cout << "dist_type not recognized." << endl; }

	for(int n:ns) {
		raw_moment[n] = sums[n] / dist_num;
		raw_moment_calc[n] = true;
	}
}


// Calculate nth order raw moment if it has not already been calculated.
void Stats::calc_raw_moment(int n) {
	vector<int> ns = {n};
	calc_raw_moment(ns);
}


// Calculate number of entries in distribution.
void Stats::calc_dist_num() {
	if(dist_type == "vec") {
		dist_num = distribution.size();
	} else if(dist_type == "hist") {
		dist_num = 0;
		for(const pair<double, int>& entry:distribution_hist) {
			dist_num += entry.second;
		}
	} else { cout << "dist_type not recognized." << endl; }
}
