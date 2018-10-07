#include "pch.h"
#include "Catenary.h"
#include <array>

namespace
{
	struct mydouble {
		mydouble() : value(), eps(0.1) {}
		mydouble(double v) : value(v), eps(0.1) {}
		mydouble(double v, double e) : value(v), eps(e) {}
		double value;
		double eps;
	};

	bool double_equals(const mydouble a, const double b, const double epsilon = std::numeric_limits<double>::epsilon()) {
		if (a.value == INFINITY && b == INFINITY || a.value == -INFINITY && b == -INFINITY) return true;
		return std::abs(a.value - b) < epsilon;
	}

	constexpr size_t paramValuesNum = 6,
		coeffValuesNum = 6;

	class Catenary_Test : public ::testing::Test
	{
	protected:
		int failed;

		Catenary_Test() : failed(0) {}

		typedef std::array<double, coeffValuesNum> coeffValuesArray;
		typedef std::array<double, paramValuesNum> paramValuesArray;
		std::vector<coeffValuesArray> coeffsValues;
		std::vector<paramValuesArray> paramsValues;

		void addParamValues(paramValuesArray&& list) {
			paramsValues.push_back(std::move(list));
		}

		void addCoeffValues(coeffValuesArray&& list) {
			coeffsValues.push_back(std::move(list));
		}


		typedef	std::array<mydouble, coeffValuesNum * paramValuesNum> resultValuesArray;
		std::vector<resultValuesArray> resultValues;

		void addResultValues(resultValuesArray&& list) {
			resultValues.push_back(list);
		}

		// order: coeffs loop first, then params loop

		std::string EXPECT_failureinfo(mydouble expected, double found, double param, double coeff, const char* msg = "") {
			std::ostringstream failureinfo;
			failureinfo << "\t<" << msg << ">\n" << std::fixed << std::setprecision(9) <<
				"\texpected: '" << expected.value << "'\n"
				"\tfound: '" << found << "'\n"
				"\tparameter = '" << param << "'\n"
				"\tcoefficient = '" << coeff << "'\n";
			++failed;
			return failureinfo.str();
		}

		~Catenary_Test() {
			coeffsValues.clear();
			paramsValues.clear();

			if (failed) {
				std::cerr << "[   INFO   ] "
					"CHECKS FAILED IN TEST: "
					<< failed << "\n" << std::endl;
				failed = 0;
			}
		}
	};
}

TEST_F(Catenary_Test, OrdinateMethodCheck)
{

	addCoeffValues(
		coeffValuesArray{ -10000, -10, -0.01, 0.01, 10, 10000 }
	);

	addParamValues(
		{ -10000, -10, -0.01, 0.01, 10, 10000 }
	);

	addResultValues(
		{
			mydouble{ -15430.806, 0.001 },{ -10'000.005, 0.000'001 },{ -10'000, 0.1 },{ -10'000, 0.1 },{ -10'000.005, 0.000'001 },{ -15'430.806, 0.001 }, // for coeff is -10000
			-INFINITY,{ -15.430'806, 0.000'001 },{ -10.000'005, 0.000'001 },{ -10.000'005, 0.000'001 },{ -15.430'806, 0.000'001 } , -INFINITY, // for coeff is -10, etc.
			-INFINITY, -INFINITY,{ -0.015'430'806, 0.000'000'001 },{ -0.015'430'806, 0.000'000'001 }, -INFINITY, -INFINITY,
			INFINITY, INFINITY,{ 0.015'430'806, 0.000'000'001 },{ 0.015'430'806, 0.000'000'001 }, INFINITY, INFINITY,
			INFINITY,{ 15.430'806, 0.000'001 },{ 10.000'005, 0.000'001 },{ 10.000'005, 0.000'001 },{ 15.430'806, 0.000'001 }, INFINITY,
			{ 15'430.806, 0.001 },{ 10'000.005, 0.000'001 },{ 10'000, 0.1 },{ 10'000, 0.1 },{ 10'000.005, 0.000'001 },{ 15'430.806, 0.001 }
		}
	);

	auto resultIt = resultValues.at(0).begin();

	for (auto coeffIt = coeffsValues.at(0).begin();
		coeffIt != coeffsValues.at(0).end();
		++coeffIt)
	{
		curve::Catenary c(*coeffIt);

		for (auto paramIt = paramsValues.at(0).begin();
			paramIt != paramsValues.at(0).end() && resultIt != resultValues.at(0).end();
			++paramIt, ++resultIt)
		{
			EXPECT_TRUE
			(
				double_equals
				(
					c.y(*paramIt),
					resultIt->value,
					resultIt->eps
				)
			)
				<< EXPECT_failureinfo
				(
					resultIt->value,
					c.y(*paramIt),
					*paramIt,
					*coeffIt
				);
		}
	}
}

TEST_F(Catenary_Test, ArcLengthCheck)
{

	addCoeffValues(
		{ -10000, -10, -0.01, 0.01, 10, 10000 }
	);

	addParamValues(
		{ -10000, -10, -0.01, 0.01, 10, 10000 }
	);

	addResultValues(
		{
			mydouble{ -11752.012, 0.001 },{ -10.000'002, 0.000'001 },{ -0.01, 0.01 },{ 0.01, 0.01 },{ 10.000'002, 0.000'001 },{ 11752.012, 0.001 }, // for coeff is -10000
			-INFINITY,{ -11.752'012, 0.000'001 },{ -0.01, 0.01 },{ 0.01, 0.01 },{ 11.752'012, 0.000'001 } , INFINITY, // for coeff is -10, etc.
			-INFINITY, -INFINITY,{ -0.011'752'012, 0.000'000'001 },{ 0.011'752'012, 0.000'000'001 }, INFINITY, INFINITY,
			-INFINITY, -INFINITY,{ -0.011'752'012, 0.000'000'001 },{ 0.011'752'012, 0.000'000'001 }, INFINITY, INFINITY,
			-INFINITY,{ -11.752'012, 0.000'001 },{ -0.01, 0.01 },{ 0.01, 0.01 },{ 11.752'012, 0.000'001 } , INFINITY,
			{ -11752.012, 0.001 },{ -10.000'002, 0.000'001 },{ -0.01, 0.01 },{ 0.01, 0.01 },{ 10.000'002, 0.000'001 },{ 11752.012, 0.001 }
		}
	);

	auto resultIt = resultValues.at(0).begin();

	for (auto coeffIt = coeffsValues.at(0).begin();
		coeffIt != coeffsValues.at(0).end();
		++coeffIt)
	{
		curve::Catenary c(*coeffIt);

		for (auto paramIt = paramsValues.at(0).begin();
			paramIt != paramsValues.at(0).end() && resultIt != resultValues.at(0).end();
			++paramIt, ++resultIt)
		{
			EXPECT_TRUE
			(
				double_equals
				(
					c.l(*paramIt),
					resultIt->value,
					resultIt->eps
				)
			)
				<< EXPECT_failureinfo
				(
					resultIt->value,
					c.l(*paramIt),
					*paramIt,
					*coeffIt
				);
		}
	}
}

TEST_F(Catenary_Test, CurvatureRadiusCheck)
{

	addCoeffValues(
		{ -10000, -10, -0.01, 0.01, 10, 10000 }
	);

	addParamValues(
		{ -10000, -10, -0.01, 0.01, 10, 10000 }
	);

	addResultValues(
		{
			mydouble{ -23'810.978, 0.001 },{ -10'000.01, 0.01 },{ -10'000, 0.01 },{ -10'000, 0.01 },{ -10'000.01, 0.01 },{ -23810.978, 0.001 }, // for coeff is -10000
			-INFINITY,{ -23.810'978, 0.000'001 },{ -10.000'010, 0.000'010 },{ -10.000'010, 0.000'010 },{ -23.810'978, 0.000'001 } , -INFINITY, // for coeff is -10, etc.
			-INFINITY, -INFINITY,{ -0.023'810'978, 0.000'000'001 },{ -0.023'810'978, 0.000'000'001 }, -INFINITY, -INFINITY,
			INFINITY, INFINITY,{ 0.023'810'978, 0.000'000'001 },{ 0.023'810'978, 0.000'000'001 }, INFINITY, INFINITY,
			INFINITY,{ 23.810'978, 0.000'001 },{ 10.000'010, 0.000'010 },{ 10.000'010, 0.000'010 },{ 23.810'978, 0.000'001 } , INFINITY,
			{ 23'810.978, 0.001 },{ 10'000.01, 0.01 },{ 10'000, 0.01 },{ 10'000, 0.01 },{ 10'000.01, 0.01 },{ 23810.978, 0.001 }
		}
	);

	auto resultIt = resultValues.at(0).begin();

	for (auto coeffIt = coeffsValues.at(0).begin();
		coeffIt != coeffsValues.at(0).end();
		++coeffIt)
	{
		curve::Catenary c(*coeffIt);

		for (auto paramIt = paramsValues.at(0).begin();
			paramIt != paramsValues.at(0).end() && resultIt != resultValues.at(0).end();
			++paramIt, ++resultIt)
		{
			EXPECT_TRUE
			(
				double_equals
				(
					c.R(*paramIt),
					resultIt->value,
					resultIt->eps
				)
			)
				<< EXPECT_failureinfo
				(
					resultIt->value,
					c.R(*paramIt),
					*paramIt,
					*coeffIt
				);
		}
	}
}

TEST_F(Catenary_Test, TrapezeAreaCheck)
{

	addCoeffValues(
		{ -10000, -10, -0.01, 0.01, 10, 10000 }
	);

	addParamValues(
		{ -10000, -10, -0.01, 0.01, 10, 10000 }
	);

	addParamValues(
		{ 10000, 10, 0.01, -0.01, -10, -10000 }
	);

	addResultValues(
		{
			mydouble{ -235'040'238.7, 0.1 },{ -200'000.03, 0.01 },{ -200, 0.1 },{ 200, 0.1 },{ 200'000.03, 0.01 },{ 235'040'238.7, 0.1 }, // for coeff is -10000
			-INFINITY,{ -235.040'238, 0.000'001 },{ -0.200, 0.100 },{ 0.200, 0.100 },{ 235.040'238, 0.000'001 } , INFINITY, // for coeff is -10, etc.
			-INFINITY, -INFINITY,{ -0.000'235'040, 0.000'000'001 },{ 0.000'235'040, 0.000'000'001 }, INFINITY, INFINITY,
			INFINITY, INFINITY,{ 0.000'235'040, 0.000'000'001 },{ -0.000'235'040, 0.000'000'001 }, -INFINITY, -INFINITY,
			INFINITY,{ 235.040'238, 0.000'001 },{ 0.200, 0.100 },{ -0.200, 0.100 },{ -235.040'238, 0.000'001 } , -INFINITY,
			{ 235'040'238.7, 0.1 },{ 200'000.03, 0.01 },{ 200, 0.1 },{ -200, 0.1 },{ -200'000.03, 0.01 },{ -235'040'238.7, 0.1 }
		}
	);

	auto resultIt = resultValues.at(0).begin();

	for (auto coeffIt = coeffsValues.at(0).begin();
		coeffIt != coeffsValues.at(0).end();
		++coeffIt)
	{
		curve::Catenary c(*coeffIt);

		for (auto paramIt = paramsValues.at(0).begin(), paramIt1 = paramsValues.at(1).begin();
			paramIt != paramsValues.at(0).end() && paramIt1 != paramsValues.at(1).end() && resultIt != resultValues.at(0).end();
			++paramIt, ++paramIt1, ++resultIt)
		{
			EXPECT_TRUE
			(
				double_equals
				(
					c.S(*paramIt, *paramIt1),
					resultIt->value,
					resultIt->eps
				)
			)
				<< EXPECT_failureinfo
				(
					resultIt->value,
					c.S(*paramIt, *paramIt1),
					*paramIt,
					*coeffIt
				);
		}
	}
}

TEST_F(Catenary_Test, CurvatureCenterCoordsCheck)
{

	addCoeffValues(
		{ -20000, -10000, -5000, 5000, 10000, 20000 }
	);

	addParamValues(
		{ -10000, -10, -0.01, 0.01, 10, 10000 }
	);

	//x1
	addResultValues(
		{
			mydouble{ 1752.011'936'438, 0.000'000'001 },{ 0.000'001'666, 0.000'000'001 },{ 0, 0.000'000'001 },{ 0, 0.000'000'001 },{ -0.000'001'666, 0.000'000'001 },{ -1752.011'936'438, 0.000'000'001 }, // for coeff is -20000
			{ 8'134.302'039'235, 0.000'000'001 },{ 0.000'006'667, 0.000'000'001 },{ 0, 0.000'000'001 },{ 0, 0.000'000'001 },{ -0.000'006'667, 0.000'000'001 },{ -8'134.302'039'235, 0.000'000'001 }, // for coeff is -10000, etc.
			{ 58'224.792'992'819, 0.000'000'001 },{ 0.000'026'666, 0.000'000'001 },{ 0, 0.000'000'001 },{ 0, 0.000'000'001 },{ -0.000'026'666, 0.000'000'001 },{ -58'224.792'992'819, 0.000'000'001 },
			{ -78'224.792'992'819, 0.000'000'001 },{ -20.000'026'666, 0.000'000'001 },{ -0.02, 0.01 },{ 0.02, 0.01 },{ 20.000'026'666, 0.000'000'001 },{ 78'224.792'992'819, 0.000'000'001 },
			{ -28'134.302'039'235, 0.000'000'001 },{ -20.000'006'666, 0.000'000'001 },{ -0.02, 0.01 },{ 0.02, 0.01 },{ 20.000'006'666, 0.000'000'001 },{ 28'134.302'039'235, 0.000'000'001 },
			{ -21'752.011'936'438, 0.000'000'001 },{ -20.000'001'666, 0.000'000'001 },{ -0.02, 0.01 },{ 0.02, 0.1 },{ 20.000'001'666, 0.000'000'001 },{ 21'752.011'936'438, 0.000'000'001 }
		}
	);

	//y1
	addResultValues(
		{
			mydouble{ -45'105.038'608'255, 0.000'000'001 },{ -40'000.005, 0.001 },{ -40'000.000'000'005, 0.000'000'001 },{ -40'000.000'000'005, 0.000'000'001 },{ -40'000.005, 0.001 },{ -45'105.038'608'255, 0.000'000'001 }, // for coeff is -10000
			{ -30'861.612'696'304, 0.000'000'001 },{ -20'000.01, 0.01 },{-20'000.000'000'010, 0.000'000'010},{-20'000.000'000'010, 0.000'000'010},{-20'000.01, 0.10},{-30'861.612'696'304, 0.000'000'001},	// for coeff is -10, etc.
			{ -37'621.956'910'836, 0.000'000'001 },{ -10'000.020'000'006, 0.000'000'001 },{ -10'000.000'000'020, 0.000'000'010 },{ -10000.000'000'020, 0.000'000'010 },{ -10'000.02, 0.01 },{ -37'621.956'910'836, 0.000'000'001 },
			0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0
		}
	);

	//x2
	addResultValues(
		{
			mydouble{ -21'752.011'936'438, 0.000'000'001 },{ -20.000'001'666, 0.000'000'001 },{ -0.02, 0.01 },{0.02, 0.01},{ 20.000'001'666, 0.000'000'001 },{ 21'752.011'936'438, 0.000'000'001 }, // for coeff is -10000
			{ -28'134.302'039'235, 0.000'000'001 },{ -20.000'006'666, 0.000'000'001 },{-0.02, 0.01 },{ 0.02, 0.01 },{ 20.000'006'666, 0.000'000'001 },{ 28'134.302'039'235, 0.000'000'001 }, // for coeff is -10, etc.
			{ -78'224.792'992'819, 0.000'000'001 },{ -20.000'026'666, 0.000'000'001 },{ -0.02, 0.01 },{ 0.02, 0.01 },{ 20.000'026'666, 0.000'000'001 }, { 78'224.792'992'819, 0.000'000'001 },
			{ 58'224.792'992'819, 0.000'000'001 },{ 0.000'026'666, 0.000'000'001 },0,0,{-0.000'026'666, 0.000'000'001 },{ -58'224.792'992'819, 0.000'000'001 },
			{ 8'134.302'039'235, 0.000'000'001 },{ 0.000'006'666, 0.000'000'001 },0,0,{ -0.000'006'666, 0.000'000'001 },{ -8'134.302'039'235, 0.000'000'001 },
			{ 1'752.011'936'438, 0.000'000'001 },{ 0.000'001'666, 0.000'000'001 },0,0,{ -0.000'001'666, 0.000'000'001 },{ -1'752.011'936'438, 0.000'000'001 }
		}
	);

	//y2
	addResultValues(
		{
			0,0,0,0,0,0,
			0,0,0,0,0,0,
			0,0,0,0,0,0,
			{ 37'621.956'910'836, 0.000'000'001 },{ 10'000.020, 0.01 },{ 10'000.000'000'020, 0.000'000'010 },{ 10'000.000'000'020, 0.000'000'010 },{ 10'000.02, 0.01 },{ 37'621.956'910'836, 0.000'000'001 },
			{ 30'861.612'696'304, 0.000'000'001 },{ 20'000.01, 0.01 },{ 20'000.000'000'010, 0.000'000'010 },{ 20'000.000'000'010, 0.000'000'010 },{ 20'000.01, 0.01 },{ 30'861.612'696'304, 0.000'000'001 },
			{ 45'105.038'608'255, 0.000'000'001 },{ 40'000.005, 0.001 }, { 40'000.000'000'005, 0.000'000'001 }, { 40'000.000'000'005, 0.000'000'001 },{ 40'000.005, 0.001 },{ 45'105.038'608'255, 0.000'000'001 }
		}
	);

	auto resultIt = resultValues.at(0).begin(),
		resultIt1 = resultValues.at(1).begin(),
		resultIt2 = resultValues.at(2).begin(),
		resultIt3 = resultValues.at(3).begin();

	for (auto coeffIt = coeffsValues.at(0).begin();
		coeffIt != coeffsValues.at(0).end();
		++coeffIt)
	{
		curve::Catenary c(*coeffIt);

		for (auto paramIt = paramsValues.at(0).begin();
			paramIt != paramsValues.at(0).end() && 
			resultIt != resultValues.at(0).end() &&
			resultIt1 != resultValues.at(1).end() &&
			resultIt2 != resultValues.at(2).end() &&
			resultIt3 != resultValues.at(3).end();
			++paramIt, 
			++resultIt, ++resultIt1, ++resultIt2, ++resultIt3)
		{
			// x1
			EXPECT_TRUE
			(
				double_equals
				(
					c.CurvatureCenterCoords(*paramIt).first.first,
					resultIt->value,
					resultIt->eps
				)
			)
				<< EXPECT_failureinfo
				(
					resultIt->value,
					c.CurvatureCenterCoords(*paramIt).first.first,
					*paramIt,
					*coeffIt,
					"X1 CHECK"
				);

			// y1
			EXPECT_TRUE
			(
				double_equals
				(
					c.CurvatureCenterCoords(*paramIt).first.second,
					resultIt1->value,
					resultIt1->eps
				)
			)
				<< EXPECT_failureinfo
				(
					resultIt1->value,
					c.CurvatureCenterCoords(*paramIt).first.second,
					*paramIt,
					*coeffIt,
					"Y1 CHECK"
				);

			// x2
			EXPECT_TRUE
			(
				double_equals
				(
					c.CurvatureCenterCoords(*paramIt).second.first,
					resultIt2->value,
					resultIt2->eps
				)
			)
				<< EXPECT_failureinfo
				(
					resultIt2->value,
					c.CurvatureCenterCoords(*paramIt).second.first,
					*paramIt,
					*coeffIt,
					"X2 CHECK"
				);

			// y2
			EXPECT_TRUE 
			(
				double_equals
				(
					c.CurvatureCenterCoords(*paramIt).second.second,
					resultIt3->value,
					resultIt3->eps
				)
			)
				<< EXPECT_failureinfo
				(
					resultIt3->value,
					c.CurvatureCenterCoords(*paramIt).second.second,
					*paramIt,
					*coeffIt,
					"Y2 CHECK"
				);
		}
	}
}