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


vector<int> get_Rs(const vector<double>& angles, int divisions) {
	vector<int> Rs(divisions, 0);
	int bin;

//	if(angles.size() > 0) {
		for(double angle:angles) {
			if(angle >= 2*M_PI || angle < 0) {
//				cout << "Angle " << angle << " out of range. Rotating into range [0,2*pi)." << endl;
				angle = rotate_angle(angle, 0);
			}
			bin = angle / (2 * M_PI) * divisions;
			Rs[bin]++;
		}
//	}

	return(Rs);
}


// Take set of angles and divide 2pi azimuth into bin_num bins of size bin_width with random spacing between bins.
// Count number of angles within each bin and return this histogram. First bin always starts at 0 so angles should be rotated beforehand.
vector<int> get_Rs(const vector<double>& angles, double bin_width, TRandom3 *r, int bin_num) {
	if(bin_width > 2*M_PI || bin_width <= 0) {   // If bin_width not in range (0, 2Pi] set to 2Pi
		cout << "get_Rs bin_width " << bin_width << " out of range, setting to 2_PI" << endl;
		bin_width = 2*M_PI;
	}
	if(bin_num > 2*M_PI / bin_width) {  // If greater than max possible bins, set to max bins.
		cout << "Asked for " << bin_num << " bins, " << flush;
		bin_num = (int) 2*M_PI / bin_width;
		cout << "setting to max of " << bin_num << endl;
	}

	vector<int> Rs(bin_num, 0);

	vector<pair<double, double>> bins(bin_num, {0, 0});
	bins[0] = {0, bin_width};
	for(int bin = 1; bin<bin_num; bin++) {
		double range = 2*M_PI - bin_width * (bin_num - bin) - bins[bin-1].second;
		double low = r->Rndm() * range + bins[bin-1].second;
		bins[bin] = {low, low+bin_width};
	}

	for(double angle:angles) {
		if(angle >= 2*M_PI || angle < 0) {
			angle = rotate_angle(angle, 0);
		}
		int bin_index = 0;
		for(pair<double, double> &bin:bins) {
			if(angle >= bin.first && angle < bin.second) {
				Rs[bin_index]++;
				break;
			}
			if(angle < bin.first) { break; }
			bin_index++;
		}
	}

	return(Rs);
}


//Rotate angle by rotate while keeping within 2pi range.
double rotate_angle(double angle, double rotate) {
	angle += rotate;
	while(angle >= 2 * M_PI) { angle -= 2 * M_PI; }
	while(angle < 0) { angle += 2 * M_PI; }
	return(angle);
}


//Rotate angles by rotate. Generalization of rotate_angle.
vector<double> rotate_angles(const vector<double>& angles, double rotate) {
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
	double mu2, mu3, mu4, mu5, mu6, mu8;
	if(n == 1) {
		mu2 = get_central_moment(data, 2);

		cumulant = get_raw_moment(data, 1);

		err = mu2;// / (double)data.size();
		err = pow(err/(double)data.size(), 0.5);
	} else if(n == 2) {
		mu2 = get_central_moment(data, 2);
		mu4 = get_central_moment(data, 4);

		cumulant = mu2;

		err = ( mu4 - pow(mu2,2) );// / (double)data.size();
		err = pow(err/(double)data.size(), 0.5);
	} else if(n == 3) {
		mu2 = get_central_moment(data, 2);
		mu3 = get_central_moment(data, 3);
		mu4 = get_central_moment(data, 4);
		mu6 = get_central_moment(data, 6);

		cumulant = mu3;

		err = ( mu6 - pow(mu3,2) + 9*pow(mu2,3) - 6*mu2*mu4 );// / (double)data.size();
		err = pow(err/(double)data.size(), 0.5);
	} else if(n == 4) {
		mu2 = get_central_moment(data, 2);
		mu3 = get_central_moment(data, 3);
		mu4 = get_central_moment(data, 4);
		mu5 = get_central_moment(data, 5);
		mu6 = get_central_moment(data, 6);
		mu8 = get_central_moment(data, 8);

		cumulant = mu4 - 3 * pow(mu2,2);

		err = ( mu8 - 12*mu6*mu2 - 8*mu5*mu3 - pow(mu4, 2) + 48*mu4*pow(mu2,2) + 64*pow(mu3,2)*mu2 - 36*pow(mu2,4) );// / (double)data.size();
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


//Calculate the nth central moment of data given the mean (faster).
double get_central_moment(vector<double> data, int n, double mean) {
	double moment = 0;
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


//Convert ratios map to vector.
TH2I* ratios_map_to_hist(map<int, map<int, int>> ratios, string name) {
	if(ratios.size() <= 0) {	return(new TH2I(name.data(), name.data(), 2, 0, 1, 2, 0, 1)); }  // Bad workaround, fix.
	int max_n = (--ratios.end())->first;
	TH2I *ratio_hist = new TH2I(name.data(), name.data(), max_n+1, -0.5, max_n+0.5, max_n+1, -0.5, max_n+0.5);
	for(pair<int, map<int, int>> event:ratios) {
		for(pair<int, int> bin:event.second) {
			ratio_hist->Fill(event.first, bin.first, bin.second);
		}
	}

	return(ratio_hist);
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


vector<double> get_two_particle_correlation(vector<double> &angles) {
	vector<double> corrs;
	int num_angles = (int) angles.size();
	for(int i=0; i<num_angles-1; i++) {
		for(int j=i+1; j<num_angles; j++) {
			double cor = angles[j] - angles[i];
			while(cor < 0) { cor+=2*M_PI; }
			while(cor > 2*M_PI) { cor-=2*M_PI; }
			corrs.push_back(cor);
		}
	}

	return(corrs);
}


double sample_sd(vector<double> data) {
	double mean = 0.0, sd = 0.0;
	for(double element:data) { mean += element; }
	mean /= (int)data.size();
	for(double element:data) { sd += pow(element - mean, 2); }
	sd = pow(sd / ((int)data.size() - 1), 0.5);

	return(sd);
}


double sample_sd(vector<Measure> data) {
	vector<double> val_vec;
	for(Measure element:data) {
		val_vec.push_back(element.get_val());
	}

	double sd = sample_sd(val_vec);

	return(sd);
}


// Get median Measure of vector by value. If even number of Measures takes lower of two median options.
Measure median(vector<Measure> data) {
	sort(data.begin(), data.end(), [] (Measure a, Measure b) { return(a.get_val() < b.get_val()); } );
	return(data[(int)(data.size()-1) / 2]);
}
