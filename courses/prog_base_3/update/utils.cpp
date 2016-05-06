#include "stdafx.h"

#include <stdlib.h>

#include "utils.h"
#include "constants.h"

namespace Utils {
	double toRads(double dgr) {
		return dgr / 180.0 * Constants::PI;
	}

	float randf(){
		return ((1.0f / 127.0f) * (((float)(rand() & 255)) - 128.0f));
	}

	double randd() {
		return ((1.0 / 127.0) * (((float)(rand() & 255)) - 128.0));
	}

	double getMantissaAndExp(const double value, int * exp) {
		bool isNegat = value < 0.0 ? true : false;
		double mantissa = isNegat ? -value: value;
		int l_exp = 0;
		
		if (mantissa == 0.0) {
			if (exp != NULL) *exp = 0;
			return 0.0;
		}

		while (mantissa >= 10.0) {
			mantissa /= 10.0;
			l_exp++;
		}
		
		if (mantissa < 0.01)
		{
			while (mantissa < 1.0) {
				mantissa *= 10.0;
				l_exp--;
			}
		}

		if (exp != NULL) *exp = l_exp;
		if (isNegat) mantissa = -mantissa;
		return mantissa;
	}
}