/*
 * ratio_methods.cpp
 *
 *  Created on: Jun 17, 2019
 *      Author: Dylan Neff
 */


#include <iostream>
#include <vector>
#include <tuple>
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
tuple<double, double> get_cumulant(vector<double> data, int n) {
	double cumulant, err;
	double mu1, mu2, mu3, mu4, mu5, mu6, mu7, mu8;
	if(n == 1) {
		mu1 = get_raw_moment(data, 1);
		mu2 = get_raw_moment(data, 2);
		cumulant = mu1;
		err = mu2 / (double)data.size();
		err = pow(err/(double)data.size(), 0.5);
	} else if(n == 2) {
		mu2 = get_raw_moment(data, 2);
		mu4 = get_raw_moment(data, 4);
		cumulant = mu2;
		err = ( mu4 - pow(mu2,2) ) / (double)data.size();
		err = pow(err/(double)data.size(), 0.5);
	} else if(n == 3) {
		mu2 = get_raw_moment(data, 2);
		mu3 = get_raw_moment(data, 3);
		mu4 = get_raw_moment(data, 4);
		mu6 = get_raw_moment(data, 6);
		cumulant = mu3;
		err = ( mu6 - pow(mu3,2) + 9*pow(mu2,3) - 6*mu2*mu4 ) / (double)data.size();
		err = pow(err/(double)data.size(), 0.5);
	} else if(n == 4) {
		mu2 = get_raw_moment(data, 2);
		mu3 = get_raw_moment(data, 3);
		mu4 = get_raw_moment(data, 4);
		mu5 = get_raw_moment(data, 5);
		mu6 = get_raw_moment(data, 6);
		mu4 = get_raw_moment(data, 8);
		cumulant = mu4 - 3 * pow(mu2,2);
		err = ( mu8 - 12*mu6*mu2 - 8*mu5*mu3 - pow(mu4, 2) + 48*mu4*pow(mu2,2) + 64*pow(mu3,2)*mu2 - 36*pow(mu2,4) ) / (double)data.size();
		err = pow(err/(double)data.size(), 0.5);
//	} else if(n == 5) {
//		cumulant = get_central_moment(data, n) - 10 * get_central_moment(data, 2) * get_central_moment(data, 3);
//		err = (get_central_moment(data, 4) - pow(get_central_moment(data,2),2)) / (double)data.size();
//		err = pow(err/(double)data.size(), 0.5);
	} else {
		cumulant = err = 0;
		cout << n << "th cumulant not implemented. Returned " << cumulant << " +- " << err << endl;
	}

	return(make_tuple(cumulant, err));
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
		for(pair<int, int> bin:event.second) {
			vector<double> entry(bin.second, ((double)bin.first) / event.first);
			vec.insert(end(vec), begin(entry), end(entry));
		}
	}

	return(vec);
}



// Sum two tree_data maps.
void sum_tree_data(tree_data *base, tree_data add) {
	for(pair<int, map<int, int>> cent:add.good_protons) {
		for(pair<int, int> num:cent.second) {
			base->good_protons[cent.first][num.first] += num.second;
		}
	}

	for(pair<int, map<int, map<int, map<int,int>>>> div:add.ratios) {
		for(pair<int, map<int, map<int, int>>> cent:div.second) {
			for(pair<int, map<int, int>> event:cent.second) {
				for(pair<int, int> bin:event.second) {
					base->ratios[div.first][cent.first][event.first][bin.first] += bin.second;
				}
			}
		}
	}
}


vector<double> get_two_particle_correlation(vector<double> angles) {
	vector<double> corrs;
	for(unsigned i=0; i<angles.size(); i++) {
		for(unsigned j=i+1; j<angles.size(); j++) {
			double cor = angles[j] - angles[i];
			if(cor < 0) { cor+=2*M_PI; }
			corrs.push_back(cor);
		}
	}

	return(corrs);
}
