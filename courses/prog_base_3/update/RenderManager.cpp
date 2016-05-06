#include "stdafx.h"

#include <list>

#include "utils.h"
#include "Vector3.h"
#include "RenderManager.h"

static std::list<SpaceObject *> spObjs;
static ControlPane * ctrlPane;
static bool isPaused = false;
static int backgroundStarsNumber = 256;

static enum DISPLAY_LIST{
	BACKGROUND_LIST_ID = 1,
	AXES_LIST_ID
};

// or DiplayManager
namespace RenderManager {

	void addSpaceObjectForRendering(SpaceObject * spObj) {
		spObjs.push_back(spObj);
	}

	void addSpaceObjectToCtrlPane(SpaceObject * spObj) {
		ctrlPane->addSpaceObject(spObj);
	}

	void init() {
		ctrlPane = ControlPane::getInstance();
	}

	void initBackground() {
		glNewList(BACKGROUND_LIST_ID, GL_COMPILE);

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);
		glColor3f(1.0f, 1.0f, 1.0f);

		glBegin(GL_POINTS);
		for (int i = 0; i < backgroundStarsNumber; ++i)
		{
			Vector3f starPos = Vector3f(Utils::randf(), Utils::randf(), Utils::randf()).normalized() * 1000.0f;
			glVertex3f(starPos.x, starPos.y, starPos.z);
		}
		glEnd();

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHTING);

		glEndList();
	}

	void initAxes() {
		glNewList(AXES_LIST_ID, GL_COMPILE);

		glDisable(GL_LIGHTING);
		glEnable(GL_LINE_STIPPLE);
		glColor3f(1.0f, 1.0f, 1.0f);
		glLineStipple(10, 0x5555);

		glBegin(GL_LINES);
			glVertex3f(-1000.0f, 0.0f, 0.0f);
			glVertex3f(1000.0f, 0.0f, 0.0f);
			glVertex3f(0.0f, -1000.0f, 0.0f);
			glVertex3f(0.0f, 1000.0f, 0.0f);
			glVertex3f(0.0f, 0.0f, -1000.0f);
			glVertex3f(0.0f, 0.0f, 1000.0f);
		glEnd();

		glDisable(GL_LINE_STIPPLE);
		glEnable(GL_LIGHTING);

		glEndList();
	}


	void updateSpaceObjects() {
		if (isPaused) return;
		for (SpaceObject * spObj : spObjs) {
			if (spObj->isBeingRendered())
				spObj->update(spObjs);
		}
	}

	void displaySpaceObjects() {
		for (SpaceObject * spObj : spObjs) {
			if (spObj->isBeingRendered())
				spObj->display();
		}
	}

	void displayBackground() {
		glCallList(BACKGROUND_LIST_ID);
	}

	void displayAxes() {
		glCallList(AXES_LIST_ID);
	}

	void pauseSpObjsRendering() {
		isPaused = true;
	}

	void resumeSpObjsRendering() {
		isPaused = false;
	}

	bool isRenderingPaused() {
		return isPaused;
	}

	void finalize() {
		for (SpaceObject * spObj : spObjs) {
			delete spObj;
		}

		delete ctrlPane;
	}
}