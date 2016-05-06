#pragma once

#include "SpaceObjects.h"

namespace RenderManager {

	void addSpaceObjectForRendering(SpaceObject * spObj);
	//void addSpaceObjectToCtrlPane(SpaceObject * spObj);

	void initBackground();
	void initAxes();
	void updateSpaceObjects();

	void displaySpaceObjects();
	void displayBackground();
	void displayAxes();

	void pauseSpObjsRendering();
	void resumeSpObjsRendering();
	bool isRenderingPaused();

	void finalize();
}