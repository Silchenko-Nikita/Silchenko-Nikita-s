#include "stdafx.h"

#include <list>

#include "RenderManager.h"



namespace RenderManager {
	static std::list<SpaceObject *> spObjs;

	void addSpaceObjectForRendering(SpaceObject * spObj) {
		spObjs.push_back(spObj);
	}


	void updateSpaceObjects() {
		for (SpaceObject * spObj : spObjs) {
			spObj->update(spObjs);
		}
	}

	void displaySpaceObjects() {
		for (SpaceObject * spObj : spObjs) {
			spObj->display();
		}
	}

	void freeMemory() {
		for (SpaceObject * spObj : spObjs) {
			delete spObj;
		}
	}
}