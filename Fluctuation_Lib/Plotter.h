/*
 * plotting_lib.h
 *
 *  Created on: Jul 31, 2019
 *      Author: Dylan Neff
 */

#ifndef PLOTTER_H_
#define PLOTTER_H_


#include <string>
#include <map>

#include "Measure.h"

using namespace std;


class Plotter {
private:
	int can_w = 800;
	int can_h = 800;

public:
	// Structors
	Plotter();
	~Plotter();

	// Getters
	int get_can_w();
	int get_can_h();

	// Setters
	void set_can_w(int w);
	void set_can_h(int h);

	// Doers
	void canvas_2d_dist(map<int, map<int, int>> dist, TFile *out_file, int div, string name);
	void athic_stat_vs_centrality(map<int, map<int, map<int, map<string, measure<double>>>>> stats, string name);
};



#endif /* PLOTTER_H_ */
