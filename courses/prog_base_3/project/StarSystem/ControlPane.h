#pragma once

#include <AntTweakBar.h>

#include "SpaceObjects.h"

class SpaceObject;

class ControlPane {
	static ControlPane * p_instance;

	TwBar * twBar;
	ControlPane() {};
public:
	static ControlPane * getInstance();
	void addSpObj(SpaceObject * spObj);
	~ControlPane();
};