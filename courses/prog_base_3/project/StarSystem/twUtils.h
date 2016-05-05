#pragma once

#include <AntTweakBar.h>

#include "Vector3.h"

namespace TwUtils {
	void moveOneGroupToAnother(TwBar * twBar, const char * moving, const char * target);

    void addDoubleVar(TwBar * twBar, const char * varName, double * var, bool isNonnegat, bool ableToSetDefault = true);

	void addBoolVarToGroup(TwBar * twBar, const char * groupName, const char * varName, bool * var);
	void addDoubleVarToGroup(TwBar * twBar, const char * groupName, const char * varName, double * var, bool isNonnegat, bool ableToSetDefault = true);
	void addColorVarToGroup(TwBar * twBar, const char * groupName, const char * varName, Vector3f * var);
	void addVector3dVarToGroup(TwBar * twBar, const char * groupName, const char * varName, Vector3d * var, bool isNonnegat, bool ableToSetDefault = true);
}