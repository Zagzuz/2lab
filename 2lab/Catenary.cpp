#include "pch.h"
#include "Catenary.h"

double curve::Catenary::S(double x1, double x2) const {
	return pow(a, 2) * (sinh(x2 / a) - sinh(x1 / a)); 
}

curve::coords_pair curve::Catenary::CurvatureCenterCoords(double x) const {

	double x_expr = (sinh(x / a) + pow(sinh(x / a), 3)) / abs(cosh(x / a) / a),
		y_expr = (1 + pow(sinh(x / a), 2)) / abs(cosh(x / a) / a);

	return std::make_pair(
		std::make_pair(x + x_expr, y(x) - y_expr),
		std::make_pair(x - x_expr, y(x) + y_expr)
	);
}

void curve::Catenary::set_a(const double ia) {
	a = ia;
	acoeff na;
	try {
		na = ia;
	}
	catch (const std::invalid_argument& e) {
		std::cerr << e.what()
			<< ", 'a' is now set to '1'"
			<< std::endl;
		a = 1;
	}
}

curve::Catenary::Catenary(double ia) : a(ia) {
	acoeff na;
	try {
		na = ia;
	}
	catch (const std::invalid_argument& e) {
		std::cerr << e.what()
			<< ", 'a' is now set to '1'"
			<< std::endl;
		a = 1;
	}
}