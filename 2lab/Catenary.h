#pragma once

#include <cmath>

namespace curve {

	typedef std::pair<double, double> coord;
	typedef std::pair<coord, coord> coords_pair;

	class Catenary {
		struct acoeff {
			double a;
			acoeff() : a(1) {}
			acoeff(double a) : a(a) {
				if (a == 0) {
					throw std::invalid_argument(
						"wrong value for 'a'"
					);
				}
			}
		};

		double a;

	public:

		Catenary() : a(1) {}
		Catenary(double ia);
		void set_a(const double ia);
		double get_a() const { return a; }
		double y(double x) const { return a * cosh(x / a); }
		double l(double x) const { return a * sinh(x / a); }
		double R(double x) const { return a * pow(cosh(x / a), 2); }
		coords_pair CurvatureCenterCoords(double x) const;
		double S(double x1, double x2) const;

	};

}