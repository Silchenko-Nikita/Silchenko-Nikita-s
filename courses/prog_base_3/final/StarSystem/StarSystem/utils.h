#pragma once

namespace Utils {
	double toRads(double dgr);
	float randf();
	double randd();
	bool randb();
	double getMantissaAndExp(double value, int * exp);
	void multiplyQuats(float quatRes[4], float quat1[4], float quat2[4]);
	void getModifMat(float quat[4], float curMat[16], float resMat[16]);
}