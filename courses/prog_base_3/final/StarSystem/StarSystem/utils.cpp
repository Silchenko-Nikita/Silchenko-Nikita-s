#include "stdafx.h"

#include <stdlib.h>
#include <math.h>

#include "utils.h"
#include "constants.h"
#include "vector3.h"

namespace Utils {
	double toRads(double dgr) {
		return dgr / 180.0 * Constants::PI;
	}

	float randf(){
		return ((1.0f / 127.0f) * (((float)(rand() % 255)) - 128.0f));
	}

	double randd() {
		return ((1.0 / 127.0) * (((float)(rand() % 255)) - 128.0));
	}

	bool randb() {
		return rand() % 2;
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

	/*void get(float quatRes[4], float mat[16]) {
		float T = mat[0] + mat[5] + mat[10] + 1;
		if (T > 0) {
			float S = 0.5 / sqrt(T);
			quatRes[3] = 0.25 / S;
			quatRes[0] = (mat[9] - mat[6]) * S;
			quatRes[1] = (mat[2] - mat[8]) * S;
			quatRes[2] = (mat[4] - mat[1]) * S;
		}	
	}*/

	void multiplyQuats(float quatRes[4], float quat1[4], float quat2[4]) {
		Vector3f quatVec, quatVec1(quat1[0], quat1[1], quat1[2]), quatVec2(quat2[0], quat2[1], quat2[2]);
		quatVec = Vector3f::crossed(quatVec1, quatVec2) + quatVec1 * quat2[3] + quatVec2 * quat1[3];
		quatVec = quatVec.normalized();

		quatRes[0] = quatVec.x;
		quatRes[1] = quatVec.y;
		quatRes[2] = quatVec.z;
		quatRes[3] = quat1[3] * quat2[3] - quatVec1.dot(quatVec2);
	}

	void getModifMat(float quat[4], float curMat[16], float resMat[16]) {
		float yy2 = 2.0f * quat[1] * quat[1];
		float xy2 = 2.0f * quat[0] * quat[1];
		float xz2 = 2.0f * quat[0] * quat[2];
		float yz2 = 2.0f * quat[1] * quat[2];
		float zz2 = 2.0f * quat[2] * quat[2];
		float wz2 = 2.0f * quat[3] * quat[2];
		float wy2 = 2.0f * quat[3] * quat[1];
		float wx2 = 2.0f * quat[3] * quat[0];
		float xx2 = 2.0f * quat[0] * quat[0];

		resMat[0 * 4 + 0] = -yy2 - zz2 + curMat[0 * 4 + 0];
		resMat[0 * 4 + 1] = xy2 + wz2;
		resMat[0 * 4 + 2] = xz2 - wy2;
		resMat[0 * 4 + 3] = 0.0f;

		resMat[1 * 4 + 0] = xy2 - wz2;
		resMat[1 * 4 + 1] = -xx2 - zz2 + curMat[1 * 4 + 1];
		resMat[1 * 4 + 2] = yz2 + wx2;
		resMat[1 * 4 + 3] = 0.0f;

		resMat[2 * 4 + 0] = xz2 + wy2;
		resMat[2 * 4 + 1] = yz2 - wx2;
		resMat[2 * 4 + 2] = -xx2 - yy2 + curMat[2 * 4 + 2];
		resMat[2 * 4 + 3] = 0.0f;

		resMat[3 * 4 + 0] = resMat[3 * 4 + 1] = resMat[3 * 4 + 2] = 0.0f;
		resMat[3 * 4 + 3] = 1.0f;
	}
}