/*
 * plotting_lib.cpp
 *
 *  Created on: Jul 31, 2019
 *      Author: Dylan Neff
 */

#define _USE_MATH_DEFINES
#include <cmath>
#include <string>
#include <map>
#include <limits>

#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <TF1.h>
#include <TLine.h>
#include <TGraph.h>
#include <TMultiGraph.h>
#include <TGraphErrors.h>
#include <TCanvas.h>

#include "Plotter.h"
#include "ratio_methods.h"

using namespace std;


// Structors
Plotter::Plotter() {
	// Nothing
}


Plotter::~Plotter() {
	// Nothing
}


// Setters
void Plotter::set_graph_option(string opt) {
	graph_option = opt;
}

void Plotter::set_line_width(double width) {
	line_width = width;
}


// Doers
void Plotter::moments_multi(map<double, map<double, Measure>> stats, string name, string multi_var_name, string x_var_name) {
	auto *can = new TCanvas(name.data(), name.data(), can_w, can_h);
	auto *mg = new TMultiGraph();
	mg->SetNameTitle(name.data(), name.data());
	double x_min = numeric_limits<double>::max();
	double x_max = numeric_limits<double>::min();
	double y_max = numeric_limits<double>::min();
	double y_min = numeric_limits<double>::max();
	int graph_index = 0;
	for(pair<double, map<double, Measure>> multi_var:stats) {
		vector<double> x_val, y_val, x_err, y_err;
		for(pair<double, Measure> x_var:multi_var.second) {
			y_val.push_back(x_var.second.get_val());
			y_err.push_back(x_var.second.get_err());
			x_val.push_back(x_var.first);
			x_err.push_back(0);
			if(y_val.back() + y_err.back() > y_max) { y_max = y_val.back() + y_err.back(); }
			if(y_val.back() - y_err.back() < y_min) { y_min = y_val.back() - y_err.back(); }
		}
		TGraphErrors *graph = new TGraphErrors((int)x_val.size(), x_val.data(), y_val.data(), x_err.data(), y_err.data());
		graph->SetNameTitle((multi_var_name + " " + to_string(multi_var.first).substr(0,4)).data());
		graph->SetMarkerStyle(marker_styles[style_type][graph_index%marker_styles[style_type].size()]);
		graph->SetMarkerColor(marker_colors[style_type][graph_index%marker_styles[style_type].size()]);
		graph->SetMarkerSize(marker_size);
		graph->SetLineWidth(line_width);
		graph->SetLineColor(marker_colors[style_type][graph_index%marker_styles[style_type].size()]);
		mg->Add(graph, graph_option.data());
		if(*min_element(x_val.begin(), x_val.end()) < x_min) { x_min = *min_element(x_val.begin(), x_val.end()); }
		if(*max_element(x_val.begin(), x_val.end()) > x_max) { x_max = *max_element(x_val.begin(), x_val.end()); }
		graph_index++;
	}
	double x_range = x_max - x_min;
	double y_range = y_max - y_min;
	mg->GetXaxis()->SetLimits(x_min - 0.1 * x_range, x_max + 0.1 * x_range);
	mg->GetXaxis()->SetRangeUser(x_min - 0.1 * x_range, x_max + 0.1 * x_range);
	mg->GetYaxis()->SetLimits(y_min - 0.1 * y_range, y_max + 0.1 * y_range);
	mg->GetYaxis()->SetRangeUser(y_min - 0.1 * y_range, y_max + 0.1 * y_range);
	mg->GetXaxis()->SetTitle(x_var_name.data());
	mg->Draw(graph_option.data()); // Multigraph memory leak, fix.
	gPad->BuildLegend(0.3, 0.21, 0.3, 0.21, "", "l");
	can->Write(name.data());
	delete can;
}
