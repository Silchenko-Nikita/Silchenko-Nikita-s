#pragma once

namespace Utils {
	double toRads(double dgr);
	float randf();
	double randd();
	bool randb();
	double getMantissaAndExp(double value, int * exp);
}