#pragma once

#include <AntTweakBar.h>

#include "SpaceObjects.h"
#include "panes.h"

class SpaceObject;
class Pane;

template<typename varType>
struct VarWithDefault{
	varType * var;
	varType defaultVal;

	VarWithDefault() {}
	VarWithDefault(varType * var, varType defaultVal) {
		this->var = var;
		this->defaultVal = defaultVal;
	}
};

struct SpaceObjectAndPane {
	SpaceObject * spObj;
	Pane * pane;
};


namespace TwCB {
	void TW_CALL setMantissa(const void *value, void *clientData);
	void TW_CALL getMantissa(void *value, void *clientData);
	void TW_CALL setExp(const void *value, void *clientData);
	void TW_CALL getExp(void *value, void * clientData);

	void TW_CALL setDefaultDouble_onButtonClick(void * clientData);
	void TW_CALL setDefaultVector3d_onButtonClick(void * clientData);
	void TW_CALL setDefaultVector3f_onButtonClick(void * clientData);
	void TW_CALL setDefaultStr_onButtonClick(void * clientData);

	void TW_CALL setDefaultStar_onButtonClick(void * clientData);
	void TW_CALL setDefaultPlanet_onButtonClick(void * clientData);
	void TW_CALL setDefaultSputnik_onButtonClick(void * clientData);

	void TW_CALL showNewSpaceObjectPane_onButtonClick(void * clientData);
	void TW_CALL hideNewSpaceObjectPane_onButtonClick(void * clientData);

	void TW_CALL createSpaceObject_onButtonClick(void * clientData);
	void TW_CALL deleteSpaceObject_onButtonClick(void * clientData);
}