#pragma once

#include <AntTweakBar.h>

#include "SpaceObjects.h"

class SpaceObject;

namespace TwCB {
	void TW_CALL spObj_setMantissa(const void *value, void *clientData);
	void TW_CALL spObj_getMantissa(void *value, void *clientData);
	void TW_CALL spObj_setExp(const void *value, void *clientData);
	void TW_CALL spObj_getExp(void *value, void *clientData);
}

namespace TwManager {
	void addSpObjToTwBar(TwBar * bar, SpaceObject * spObj);
}