/*
 * fourier_approx.cpp
 *
 *  Created on: Jul 9, 2019
 *      Author: Dylan Neff
 */


#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <vector>
#include <tuple>

#include <TH1.h>
#include <TF1.h>

#include "FourierApprox.h"

using namespace std;


FourierApprox::FourierApprox(TH1D *hist) {
	hist_to_approx = hist;
	fourier_n_min = 1;
	fourier_n_max = 10;
	int low_bin = hist->FindFirstBinAbove(-1);
	int high_bin = hist->FindLastBinAbove(-1);
	hist_x_min = hist->GetXaxis()->GetBinCenter(low_bin) - hist->GetXaxis()->GetBinWidth(low_bin) / 2.0;
	hist_x_max = hist->GetXaxis()->GetBinCenter(high_bin) + hist->GetXaxis()->GetBinWidth(high_bin) / 2.0;
}


FourierApprox::~FourierApprox() {
	//Nothing
}


TF1* FourierApprox::get_approx() {
	coefs = calc_coef(hist_to_approx, fourier_n_max, hist_x_min, hist_x_max);
	expansion_terms = get_expansion(coefs.first, coefs.second, fourier_n_max, hist_x_min, hist_x_max);
	TF1 *approx_func = get_func(expansion_terms, hist_x_min, hist_x_max);

	return(approx_func);
}

void FourierApprox::set_fourier_n_min(int n_min) {
	fourier_n_min = n_min;
}

void FourierApprox::set_fourier_n_max(int n_max) {
	fourier_n_max = n_max;
}

int FourierApprox::get_fourier_n_min() {
	return(fourier_n_min);
}

int FourierApprox::get_fourier_n_max() {
	return(fourier_n_max);
}

void FourierApprox::print_coefs() {
	map<int, double> a = coefs.first;
	map<int, double> b = coefs.second;
	for(int n = 0; n <= fourier_n_max; n++) {
		cout << "a[" << n << "] = " << a[n] << "  |  b[" << n << "] = " << b[n] << endl;
	}
}


//Get average of TF1 func on histogram hist.
double get_avg(TH1D *hist, TF1 *func) {
	int norm = 0;
	double sum = 0.0;
	int binCont;
	double binCent;

	for(int i = hist->GetXaxis()->GetFirst(); i<=hist->GetXaxis()->GetLast(); i++) {
		binCont = hist->GetBinContent(i);
		binCent = hist->GetBinCenter(i);
		norm += binCont;
		sum += binCont * func->Eval(binCent);
	}

	return(sum / norm);
}


//Calculate coefficients of Fourier expansion.
pair<map<int, double>, map<int, double>> calc_coef(TH1D* hist, int n_max, double x_min, double x_max) {
	map<int, double> a, b;

	double x_avg = (x_max + x_min) / 2.0; //Calculate average of range
	double L = (x_max - x_min) / 2.0; //Calculate half range (2*L is full range)

	TF1 *cos_shift = new TF1("cos_shift", "cos([3] * [2] * (x-[0]) / [1])");
	cos_shift->SetParameters(x_avg, L, M_PI, 0);
	TF1 *sin_shift = new TF1("sin_shift", "sin([3] * [2] * (x-[0]) / [1])");
	sin_shift->SetParameters(x_avg, L, M_PI, 0);

	for(int n = 0; n <= n_max; n++) {
		cos_shift->SetParameter(3, n);
		sin_shift->SetParameter(3, n);
		a[n] = get_avg(hist, cos_shift) / M_PI;
		b[n] = get_avg(hist, sin_shift) / M_PI;
	}

	a[0] /= 2;

	delete cos_shift;
	delete sin_shift;

	return(make_pair(a, b));
}


vector<TF1*> get_expansion(map<int, double> a, map<int, double> b, int n_max, double x_min, double x_max) {
	double x_avg = (x_max + x_min) / 2.0; //Calculate average of range
	double L = (x_max - x_min) / 2.0; //Calculate half range (2*L is full range)

//	TF1 *approx_func = new TF1("approx_func", "");
	vector<TF1*> funcs;

	for(int n=0; n<=n_max; n++) {
		TF1 *cos_shift = new TF1(("cos_shift" + to_string(n)).data(), "[4] * cos([3] * [2] * (x-[0]) / [1])", x_min, x_max);
		cos_shift->SetParameters(x_avg, L, M_PI, n, a[n]);
		TF1 *sin_shift = new TF1(("sin_shift" + to_string(n)).data(), "[4] * sin([3] * [2] * (x-[0]) / [1])", x_min, x_max);
		sin_shift->SetParameters(x_avg, L, M_PI, n, b[n]);
		funcs.push_back(cos_shift);
		funcs.push_back(sin_shift);
	}

	return(funcs);
}


TF1* get_func(vector<TF1*> expansion_terms, double x_min, double x_max) {
	string func_expression = "";
	for(unsigned i = 0; i < expansion_terms.size(); i++) {
		func_expression += (string) expansion_terms[i]->GetName() + "+";
	}
	func_expression = func_expression.substr(0, func_expression.size()-1);
	cout << func_expression << endl;
	TF1 *approx_func = new TF1("Approx_func", func_expression.data(), x_min, x_max);

	cout << approx_func->Eval(3.0) << " | " << approx_func->Eval(3.5) << endl;

	return(approx_func);
}
