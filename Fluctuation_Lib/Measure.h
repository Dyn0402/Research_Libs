/*
 * Measure.h
 *
 *  Created on: Jul 30, 2019
 *      Author: Dylan Neff
 */

#ifndef MEASURE_H_
#define MEASURE_H_

#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include <cmath>

using namespace std;


class Measure {
private:
	double val, err;
public:
	// Constructors
	Measure() {
		val = err = 0.0;
	}

	Measure(double val_in) {
		val = val_in;
		err = 0.0;
	}

	Measure(double val_in, double err_in) {
		val = val_in;
		err = err_in;
	}

	// Destructor
	~Measure() {
		// Nothing
	}


	// Operators
	Measure operator + (Measure const &obj) const {
		Measure result;
		result.set_val(val + obj.val);
		result.set_err(std::pow(std::pow(err, 2) + std::pow(obj.err, 2), 0.5));

		return result;
	}

	Measure operator + (double d) const {
		Measure result;
		result.set_val(val + d);
		result.set_err(err);

		return result;
	}

	Measure operator - (Measure const &obj) const {
		Measure result;
		result.set_val(val - obj.val);
		result.set_err(std::pow(std::pow(err, 2) + std::pow(obj.err, 2), 0.5));

		return result;
	}

	Measure operator - (double d) const {
		Measure result;
		result.set_val(val - d);
		result.set_err(err);

		return result;
	}

	Measure operator * (Measure const &obj) const {
		Measure result;
		result.set_val(val * obj.val);
		result.set_err(fabs(result.get_val()) * std::pow(std::pow(err/val, 2) + std::pow(obj.err/obj.val, 2), 0.5));

		return result;
	}

	Measure operator * (double d) const {
		Measure result;
		result.set_val(val * d);
		result.set_err(err * fabs(d));

		return result;
	}

	Measure operator / (Measure const &obj) const {
		Measure result;
		result.set_val(val / obj.val);
		result.set_err(fabs(result.get_val()) * std::pow(std::pow(err/val, 2) + std::pow(obj.err/obj.val, 2), 0.5));

		return result;
	}

	Measure operator / (double d) const {
		Measure result;
		result.set_val(val / d);
		result.set_err(err / fabs(d));

		return result;
	}

	Measure pow(double exp) const {
		Measure result;
		result.set_val(std::pow(val, exp));
		result.set_err(fabs(result.get_val() * exp * err / val));

		return result;
	}

	operator std::string() const {
		std::ostringstream os("");
		os << std::setprecision(15) << val << " ±" << std::setprecision(15) << err << std::ends;
		return os.str();
	}


	// Getters
	double get_val() {
		return val;
	}

	double get_err() {
		return err;
	}


	// Setters
	void set_val(double val_in) {
		val = val_in;
	}

	void set_err(double err_in) {
		err = err_in;
	}

};


#endif /* MEASURE_H_ */
