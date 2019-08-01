/*
 * plotting_lib.cpp
 *
 *  Created on: Jul 31, 2019
 *      Author: Dylan Neff
 */


#include <string>
#include <map>

#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <TF1.h>
#include <TCanvas.h>

#include "Plotter.h"
#include "ratio_methods.h"

using namespace std;



Plotter::Plotter() {
	// Nothing
}


Plotter::~Plotter() {
	// Nothing
}


void canvas_2d_dist(map<int, map<int, int>> dist, TFile *out_file, int div, string name) {
	out_file->cd();
	TH2I *hist = ratios_map_to_hist(dist, name);
	TCanvas *can = new TCanvas(name.data()); // Memory leak
	TF1 *avg_line = new TF1(name.data(), ("x/"+to_string(div)).data(), -0.5, 0.5+(--dist.end())->first);
	TF1 *max_line = new TF1((name+"max").data(), "x", -0.5, 0.5+(--dist.end())->first);
	max_line->SetLineColor(4);
	can->SetLogz();
	hist->Draw("colz");
	max_line->Draw("same");
	avg_line->Draw("same");
	hist->Write();
	can->Write();
}


void moments_multi(map<double, map<double, map<double, map<string, Measure>>>> stats, string name) {
	int div = 6; //Hardcode, fix.
	auto *can = new TCanvas(name.data(), name.data(), plot::canvas_width, plot::canvas_height);
	can->Divide(ceil(pow(data.size(),0.5)), ceil(data.size()/ceil(pow(data.size(), 0.5))));
	int can_index = 1;
	for(string stat:analysis::stat_names) {
		can->cd(can_index);
		auto *mg = new TMultiGraph();
		mg->SetNameTitle(stat.data(), stat.data());
		double x_min = numeric_limits<double>::max();
		double x_max = numeric_limits<double>::min();
		double y_max = numeric_limits<double>::min();
		double y_min = numeric_limits<double>::max();
		for(int energy:analysis::energy_list) {
			vector<double> stat_vals, cent_val, stat_err, cent_err;
			measure<double> stat_meas;
			for(int cent_index=15; cent_index > 5; cent_index--) {
				cent_val.push_back((15-cent_index)*plot::centrality_slope + plot::centrality_intercept);
				cent_err.push_back(0.0);
				stat_meas = stats[energy][div][cent_index][stat];
				stat_vals.push_back(stat_meas.val);
				stat_err.push_back(stat_meas.err);
				if(stat_meas.val + stat_meas.err > y_max) { y_max = stat_meas.val + stat_meas.err; }
				if(stat_meas.val - stat_meas.err < y_min) { y_min = stat_meas.val - stat_meas.err; }
			}
			TGraphErrors *graph = graph_x_vs_y_err(cent_val, stat_vals, cent_err, stat_err);
			graph->SetNameTitle((to_string(plot::energy_match[energy]).substr(0,4) + " GeV").data());
			graph->SetMarkerStyle(plot::energy_marker_styles[energy]);
			graph->SetMarkerColor(plot::energy_marker_colors[energy]);
			graph->SetMarkerSize(plot::energy_marker_sizes[energy]);
			mg->Add(graph, "AP");
			if(*min_element(cent_val.begin(), cent_val.end()) < x_min) { x_min = *min_element(cent_val.begin(), cent_val.end()); }
			if(*max_element(cent_val.begin(), cent_val.end()) > x_max) { x_max = *max_element(cent_val.begin(), cent_val.end()); }
		}
		double x_range = x_max - x_min;
		double y_range = y_max - y_min;
		mg->GetXaxis()->SetLimits(x_min - 0.1 * x_range, x_max + 0.1 * x_range);
		mg->GetXaxis()->SetRangeUser(x_min - 0.1 * x_range, x_max + 0.1 * x_range);
		mg->GetYaxis()->SetLimits(y_min - 0.1 * y_range, y_max + 0.1 * y_range);
		mg->GetYaxis()->SetRangeUser(y_min - 0.1 * y_range, y_max + 0.1 * y_range);
		mg->GetXaxis()->SetTitle("Centrality (%)");
		mg->Draw("AP"); // Multigraph memory leak, fix.
		if(stat == "mean") {
//			TLegend *leg = new TLegend();
//			leg->Draw();
			gPad->BuildLegend(0.3, 0.21, 0.3, 0.21, "", "p");
		}
		can_index++;
	}
	can->Write(name.data());
	delete can;
}
