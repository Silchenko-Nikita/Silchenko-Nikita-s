#pragma once

#include <AntTweakBar.h>

#include "SpaceObjects.h"

class SpaceObject;

class ControlPane {
	static ControlPane * p_instance;

	TwBar * twBar;
	
	void init();
	ControlPane() {};
public:
	static ControlPane * getInstance();
	void addSpObj(SpaceObject * spObj);
	~ControlPane();
};