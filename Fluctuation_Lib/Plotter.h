/*
 * plotting_lib.h
 *
 *  Created on: Jul 31, 2019
 *      Author: Dylan Neff
 */

#ifndef PLOTTER_H_
#define PLOTTER_H_


#include <vector>
#include <string>
#include <map>

#include <TFile.h>

#include "Measure.h"

using namespace std;


class Plotter {
private:
	int can_w = 800;
	int can_h = 800;

	string style_type = "default";

	map<string, vector<int>> marker_styles = { {"mean", {24, 25, 26, 27, 28, 30} }, {"default", {1, 1, 1, 1, 1, 1, 1, 1} } };
	map<string, vector<int>> marker_colors = { {"mean", {1, 2, 8, 4, 6, 9} }, {"default", {1, 2, 8, 4, 6, 9} } };

	string graph_option = "APL";

	double marker_size = 1;
	double line_width = 2;

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
	void set_graph_option(string opt);
	void set_line_width(double width);

	// Doers
	void moments_multi(map<double, map<double, Measure>> stats, string name, string multi_var_name, string x_var_name);
};



#endif /* PLOTTER_H_ */
