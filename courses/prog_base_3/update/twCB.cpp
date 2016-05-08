#include "stdafx.h"

#include "utils.h"
#include "twCB.h"

#include <stdlib.h>


namespace TwCB {
	void TW_CALL setMantissa(const void *value, void *clientData) {
		double * doubleVar = static_cast<double *>(clientData);
		int exp = 0;

		Utils::getMantissaAndExp(*doubleVar, &exp);

		*doubleVar = *static_cast<const double *>(value) * pow(10, exp);
	}

	void TW_CALL getMantissa(void *value, void *clientData) {
		double * doubleVar = static_cast<double *>(clientData);
		double mantissa;

		mantissa = Utils::getMantissaAndExp(*doubleVar, NULL);

		*static_cast<double *>(value) = mantissa;
	}


	void TW_CALL setExp(const void *value, void *clientData) {
		double * doubleVar = static_cast<double *>(clientData);
		double mantissa;

		mantissa = Utils::getMantissaAndExp(*doubleVar, NULL);

		*doubleVar = mantissa * pow(10, *static_cast<const int *>(value));
	}

	void TW_CALL getExp(void *value, void * clientData) {
		double * doubleVar = static_cast<double *>(clientData);
		int exp = 0;

		Utils::getMantissaAndExp(*doubleVar, &exp);

		*static_cast<int *>(value) = exp;
	}

	//

	void TW_CALL setDefaultDouble_onButtonClick(void * clientData) {
		VarWithDefault<double> * v = static_cast<VarWithDefault<double> *>(clientData);
		*v->var = v->defaultVal;
	}

	void TW_CALL setDefaultVector3d_onButtonClick(void * clientData) {
		VarWithDefault<Vector3d> * v = static_cast<VarWithDefault<Vector3d> *>(clientData);
		*v->var = v->defaultVal;
	}

	void TW_CALL setDefaultVector3f_onButtonClick(void * clientData) {
		VarWithDefault<Vector3f> * v = static_cast<VarWithDefault<Vector3f> *>(clientData);
		*v->var = v->defaultVal;
	}

	void TW_CALL setDefaultStr_onButtonClick(void * clientData) {
		VarWithDefault<std::string> * v = static_cast<VarWithDefault<std::string> *>(clientData);
		*v->var = v->defaultVal;
	}

	void TW_CALL setDefaultStar_onButtonClick(void * clientData) {
		VarWithDefault<Star> * v = static_cast<VarWithDefault<Star> *>(clientData);
		*v->var = v->defaultVal;
	}

	void TW_CALL setDefaultPlanet_onButtonClick(void * clientData) {
		VarWithDefault<Planet> * v = static_cast<VarWithDefault<Planet> *>(clientData);
		*v->var = v->defaultVal;
	}

	void TW_CALL setDefaultSputnik_onButtonClick(void * clientData) {
		VarWithDefault<Sputnik> * v = static_cast<VarWithDefault<Sputnik> *>(clientData);
		*v->var = v->defaultVal;
	}

	void TW_CALL showNewSpaceObjectPane_onButtonClick(void * clientData)
	{
		static_cast<NewSpaceObjectPane *>(clientData)->show();
	}

	void TW_CALL hideNewSpaceObjectPane_onButtonClick(void * clientData) {
		static_cast<NewSpaceObjectPane *>(clientData)->hide();
	}
	
	void TW_CALL createSpaceObject_onButtonClick(void * clientData) {
		NewSpaceObjectPane * newSpObjPane = static_cast<NewSpaceObjectPane *>(clientData);
		newSpObjPane->createSpaceObject();
		newSpObjPane->hide();
	}

	void TW_CALL deleteSpaceObject_onButtonClick(void * clientData) {
		SpaceObjectAndPane * v = static_cast<SpaceObjectAndPane *>(clientData);
		(v->pane)->removeSpaceObject(v->spObj);
		(v->spObj)->setBeingRendered(false);
		delete v;
	}
}