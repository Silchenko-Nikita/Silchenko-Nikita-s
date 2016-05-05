#include "stdafx.h"

#include <list>

#include "RenderManager.h"

static std::list<SpaceObject *> spObjs;
static bool isPaused = false;

// or DiplayManager
namespace RenderManager {

	void addSpaceObjectForRendering(SpaceObject * spObj) {
		spObjs.push_back(spObj);
	}


	void updateSpaceObjects() {
		if (isPaused) return;
		for (SpaceObject * spObj : spObjs) {
			spObj->update(spObjs);
		}
	}

	void displaySpaceObjects() {
		for (SpaceObject * spObj : spObjs) {
			spObj->display();
		}
	}

	void pauseRendering() {
		isPaused = true;
	}

	void resumeRendering() {
		isPaused = false;
	}

	void freeMemory() {
		for (SpaceObject * spObj : spObjs) {
			delete spObj;
		}
	}
}