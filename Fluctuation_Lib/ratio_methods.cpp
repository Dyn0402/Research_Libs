/*
 * ratio_methods.cpp
 *
 *  Created on: Jun 17, 2019
 *      Author: Dylan Neff
 */


#include <iostream>
#include <vector>
#include <math.h>
#include <cmath>

#include "ratio_methods.h"

using namespace std;


vector<int> get_Rs(vector<double> angles, int divisions) {
	vector<int> Rs = {};
	int r;
	double low_cut, high_cut;

	if(angles.size() > 0) {
		for(int i = 0; i < divisions; i++) {
			r = 0.0;
			low_cut = i * 2. * M_PI / divisions;
			high_cut = (i+1) * 2. * M_PI / divisions;

			for(auto angle:angles) {
				if(angle >= low_cut  && angle < high_cut) {
					r++;
				}
				if(i+1 == divisions && angle == high_cut) {
					r++;
				}
			}
			Rs.push_back(r);
		}
	}

	return(Rs);
}


//Rotate angle by rotate while keeping within 2pi range.
double rotate_angle(double angle, double rotate) {
	angle += rotate;
	if(angle > 2 * M_PI) {
		angle -= 2 * M_PI;
	}
	return(angle);
}


//Rotate angles by rotate. Generalization of rotate_angle.
vector<double> rotate_angles(vector<double> angles, double rotate) {
	vector<double> rotated_angles;
	for(double angle:angles) {
		rotated_angles.push_back(rotate_angle(angle, rotate));
	}
	return(rotated_angles);
}


//Get the nth cumulant of data. Only implemented for n = {1,2,3,4,5}
//Bad implementation, should use generating function or standard library which does.
double get_cumulant(vector<double> data, int n) {
	double cumulant;
	if(n == 1) {
		cumulant = get_raw_moment(data, n);
	} else if(n == 2 || n == 3) {
		cumulant = get_central_moment(data, n);
	} else if(n == 4) {
		cumulant = get_central_moment(data, n) - 3 * pow(get_central_moment(data, 2),2);
	} else if(n == 5) {
		cumulant = get_central_moment(data, n) - 10 * get_central_moment(data, 2) * get_central_moment(data, 3);
	} else {
		cumulant = 0;
		cout << n << "th cumulant not implemented. Returned " << cumulant << endl;
	}

	return(cumulant);
}

//Calculate the nth central moment of data.
double get_central_moment(vector<double> data, int n) {
	double moment = 0;
	double mean = get_raw_moment(data, 1);
	for(double x:data) {
		moment += pow((x - mean), n);
	}

	return(moment / data.size());
}


//Calculate nth raw moment of data.
//n=1 is mean
double get_raw_moment(vector<double> data, int n) {
	double moment = 0;
	for(double x:data) {
		moment += pow(x, n);
	}

	return(moment / data.size());
}


//Convert nproton map to vector.
vector<int> nproton_map_to_vec(map<int, int> nproton) {
	vector<int> vec;
	for(pair<int, int> num:nproton) {
		vector<int> entry(num.second, num.first);
		vec.insert(end(vec), begin(entry), end(entry));
	}

	return(vec);
}


//Convert ratios map to vector.
vector<double> ratios_map_to_vec(map<int, map<int, int>> ratios) {
	vector<double> vec;
	for(pair<int, map<int, int>> event:ratios) {
		for(pair<int, int> div:event.second) {
			vector<double> entry(div.second, ((double)div.first) / event.first);
			vec.insert(end(vec), begin(entry), end(entry));
		}
	}

	return(vec);
}

