/*
 * ratio_methods.h
 *
 *  Created on: Jun 17, 2019
 *      Author: Dylan Neff
 */

#ifndef RATIO_METHODS_H_
#define RATIO_METHODS_H_

#include <vector>

using namespace std;

vector<int> get_Rs(vector<double> angles, int divisions);
double get_cumulant(vector<double> data, int n);
double get_central_moment(vector<double> data, int n);
double get_raw_moment(vector<double> data, int n);


#endif /* RATIO_METHODS_H_ */
