#include "stdafx.h"

#include "utils.h"
#include "twUtils.h"

#include <stdlib.h>

static struct DoubleWithDefault{
	double * var;
	double defaultVal;
};

static void TW_CALL _TwCB_setMantissa(const void *value, void *clientData) {
	double * doubleVar = static_cast<double *>(clientData);
	int exp = 0;

	Utils::getMantissaAndExp(*doubleVar, &exp);

	*doubleVar = *static_cast<const double *>(value) * pow(10, exp);
}

static void TW_CALL _TwCB_getMantissa(void *value, void *clientData) {
	double * doubleVar = static_cast<double *>(clientData);
	double mantissa;

	mantissa = Utils::getMantissaAndExp(*doubleVar, NULL);

	*static_cast<double *>(value) = mantissa;
}


static void TW_CALL _TwCB_setExp(const void *value, void *clientData) {
	double * doubleVar = static_cast<double *>(clientData);
	double mantissa;

	mantissa = Utils::getMantissaAndExp(*doubleVar, NULL);

	*doubleVar = mantissa * pow(10, *static_cast<const int *>(value));
}

static void TW_CALL _TwCB_getExp(void *value, void * clientData) {
	double * doubleVar = static_cast<double *>(clientData);
	int exp = 0;

	Utils::getMantissaAndExp(*doubleVar, &exp);

	*static_cast<int *>(value) = exp;
}

static void TW_CALL _TwCB_setDefaultDouble(void * clientData) {
	DoubleWithDefault * v = static_cast<DoubleWithDefault *>(clientData);
	*v->var = v->defaultVal;
}

namespace TwUtils {
	void moveOneGroupToAnother(TwBar * twBar, const char * moving, const char * target)
	{
		char strBuff[256];
		sprintf(strBuff, "'%s'/%s group= %s", TwGetBarName(twBar), moving, target);
		TwDefine(strBuff);
	}

	void addDoubleVar(TwBar * twBar, const char * varName, double * var, bool isNonnegat, bool ableToSetDefault)
	{
		char strBuff[256], strBuff1[256];

		sprintf(strBuff, "%s_mantissa", varName);
		sprintf(strBuff1, "label='mantissa' group='%s' min=%s max=10.0 step=0.02", varName, isNonnegat ? "0.0" : "-10.0");
		TwAddVarCB(twBar, strBuff, TW_TYPE_DOUBLE, _TwCB_setMantissa, _TwCB_getMantissa, var, strBuff1);

		sprintf(strBuff, "%s_exponent", varName);
		sprintf(strBuff1, "label='exponent' group='%s'", varName);
		TwAddVarCB(twBar, strBuff, TW_TYPE_INT32, _TwCB_setExp, _TwCB_getExp, var, strBuff1);

		if (ableToSetDefault) {

			DoubleWithDefault * v = new DoubleWithDefault();
			v->var = var;
			v->defaultVal = *var;

			sprintf(strBuff, "%s_setDefault", varName);
			sprintf(strBuff1, "label='setDefault' group='%s'", varName);
			TwAddButton(twBar, strBuff, _TwCB_setDefaultDouble, v, strBuff1);
		}
	}

	void addBoolVarToGroup(TwBar * twBar, const char * groupName, const char * varName, bool * var) {
		char strBuff[256], strBuff1[256];

		sprintf(strBuff, "%s_%s", groupName, varName);
		TwAddVarRW(twBar, strBuff, TW_TYPE_BOOLCPP, var, "");
		sprintf(strBuff1, "'%s'/%s label='%s' group='%s'", TwGetBarName(twBar), strBuff, varName, groupName);
		TwDefine(strBuff1);
	}

	void addDoubleVarToGroup(TwBar * twBar, const char * groupName, const char * varName, double * var, bool isNonnegat, bool ableToSetDefault)
	{
		char strBuff[256], strBuff1[256];

		sprintf(strBuff, "%s_%s", groupName, varName);
		addDoubleVar(twBar, strBuff, var, isNonnegat, ableToSetDefault);
		sprintf(strBuff1, "'%s'/%s label='%s' opened=false", TwGetBarName(twBar), strBuff, varName);
		TwDefine(strBuff1);
		moveOneGroupToAnother(twBar, strBuff, groupName);
	}

	void addColorVarToGroup(TwBar * twBar, const char * groupName, const char * varName, Vector3f * var)
	{
		char strBuff[256], strBuff1[256];

		sprintf(strBuff, "%s_%s", groupName, varName);
		TwAddVarRW(twBar, strBuff, TW_TYPE_COLOR3F, var, "");
		sprintf(strBuff1, "'%s'/%s opened=false label='%s'", TwGetBarName(twBar), strBuff, groupName, varName);
		TwDefine(strBuff1);
		moveOneGroupToAnother(twBar, strBuff, groupName);
	}

	void addVector3dVarToGroup(TwBar * twBar, const char * groupName, const char * varName, Vector3d * var, bool isNonnegat, bool ableToSetDefault)
	{
		char strBuff[256], strBuff1[256];
		sprintf(strBuff, "%s_%s", groupName, varName);

		addDoubleVarToGroup(twBar, strBuff, "x", &var->x, isNonnegat, ableToSetDefault);
		addDoubleVarToGroup(twBar, strBuff, "y", &var->y, isNonnegat, ableToSetDefault);
		addDoubleVarToGroup(twBar, strBuff, "z", &var->z, isNonnegat, ableToSetDefault);

		sprintf(strBuff1, "'%s'/%s label='%s' opened=false", TwGetBarName(twBar), strBuff, varName);
		TwDefine(strBuff1);

		moveOneGroupToAnother(twBar, strBuff, groupName);
	}

}