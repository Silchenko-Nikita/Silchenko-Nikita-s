#pragma once

#include "SpaceObjects.h"

namespace RenderManager {

	void addSpaceObjectForRendering(SpaceObject * spObj);
	void updateSpaceObjects();
	void displaySpaceObjects();
	void freeMemory();
	void pauseRendering();
	void resumeRendering();
}