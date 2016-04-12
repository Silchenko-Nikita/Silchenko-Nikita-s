#include "stdafx.h"

#include "utils.h"
#include "constants.h"

namespace Utils {
	double toRads(double dgr) {
		return dgr / 180.0 * Constants::PI;
	}

	double getMantissaAndExp(const double value, int * exp) {
		double mantissa = value;
		int l_exp = 0;
		
		while (mantissa >= 10.0) {
			mantissa /= 10;
			l_exp++;
		}
		while (mantissa < 1.0) {
			mantissa *= 10;
			l_exp--;
		}

		if (exp != NULL) *exp = l_exp;

		return mantissa;
	}
}