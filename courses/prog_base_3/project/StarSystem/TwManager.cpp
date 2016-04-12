#include "stdafx.h"

#include <string>

#include "SpaceObjects.h"
#include "utils.h"

// to be refractored

namespace TwManager {
	static void moveOneGroupToAnother(TwBar * bar, const char * moving, const char * target) {
		std::string defStr = "'";
		defStr += TwGetBarName(bar);
		defStr += "'/";
		defStr += moving;
		defStr += " group=";
		defStr += target;

		TwDefine(defStr.c_str());
	}

	void addSpObjToTwBar(TwBar * bar, SpaceObject * spObj){
		std::string str = "'";
		str += TwGetBarName(bar);
		str += "' label='";
		str += TwGetBarName(bar);
		str += " (SI metrics)'";
		
		TwDefine(str.c_str());

		TwAddVarCB(bar, "mass_mantissa", TW_TYPE_DOUBLE, TwCB::spObj_setMantissa, TwCB::spObj_getMantissa, &spObj->mass, " label='mantissa' group='mass' min=0.0 max=9.9 step=0.05");
		TwAddVarCB(bar, "mass_exponent", TW_TYPE_INT32, TwCB::spObj_setExp, TwCB::spObj_getExp, &spObj->mass, " label='exponent' group='mass' min=0 max=40 step=1");
		

		moveOneGroupToAnother(bar, "mass", spObj->getName());
	
		//

		TwAddVarCB(bar, "diameter_mantissa", TW_TYPE_DOUBLE, TwCB::spObj_setMantissa, TwCB::spObj_getMantissa, &spObj->diameter, " label='mantissa' group='diameter' min=0.0 max=9.9 step=0.05");
		TwAddVarCB(bar, "diameter_exponent", TW_TYPE_INT32, TwCB::spObj_setExp, TwCB::spObj_getExp, &spObj->diameter, " label='exponent' group='diameter' min=0 max=40 step=1");

		moveOneGroupToAnother(bar, "diameter", spObj->getName());
		
		str = spObj->getTypeStr();
		str += 's';
		moveOneGroupToAnother(bar, spObj->getName(), str.c_str());
	}
}

namespace TwCB {
	void TW_CALL spObj_setMantissa(const void *value, void *clientData) {
		double * spObj_field = static_cast<double *>(clientData);
		int exp = 0;

		Utils::getMantissaAndExp(*spObj_field, &exp);

		*spObj_field = *static_cast<const double *>(value) * pow(10, exp);
	}

	void TW_CALL spObj_getMantissa(void *value, void *clientData) {
		double * spObj_field = static_cast<double *>(clientData);
		double mantissa;

		mantissa = Utils::getMantissaAndExp(*spObj_field, NULL);

		*static_cast<double *>(value) = mantissa;
	}

	void TW_CALL spObj_setExp(const void *value, void *clientData) {
		double * spObj_field = static_cast<double *>(clientData);
		double mantissa;

		mantissa = Utils::getMantissaAndExp(*spObj_field, NULL);

		*spObj_field = mantissa * pow(10, *static_cast<const int *>(value));
	}

	void TW_CALL spObj_getExp(void *value, void * clientData) {
		double * spObj_field = static_cast<double *>(clientData);
		int exp = 0;

		Utils::getMantissaAndExp(*spObj_field, &exp);

		*static_cast<int *>(value) = exp;
	}
}