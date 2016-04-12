#pragma once

namespace Utils {
	double toRads(double dgr);
	double getMantissaAndExp(double value, int * exp);
	//static constexpr double operator "" _rads(long double dgr);
}