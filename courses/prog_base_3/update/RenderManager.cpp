#include "stdafx.h"

#include "utils.h"
#include "Vector3.h"
#include "RenderManager.h"


RenderManager * RenderManager::instance = NULL;
const int RenderManager::backgroundStarsNumber = 256;

RenderManager::RenderManager() {
	isPaused = false;
}

RenderManager * RenderManager::getInstance()
{
	if (NULL != instance) return NULL;

	instance = new RenderManager();
	
	return instance;
}

void RenderManager::renderSpaceObject(SpaceObject * spObj) {
	spObjsList.push_back(spObj);
}

void RenderManager::initBackground() {
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

void RenderManager::initAxes() {
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


void RenderManager::updateSpaceObjects() {
	if (isPaused) return;
	for (SpaceObject * spObj : spObjsList) {
		if (spObj->isBeingRendered())
			spObj->update(spObjsList);
	}
}

void RenderManager::displaySpaceObjects() {
	for (SpaceObject * spObj : spObjsList) {
		if (spObj->isBeingRendered())
			spObj->display();
	}
}

void RenderManager::displayBackground() {
	glCallList(BACKGROUND_LIST_ID);
}

void RenderManager::displayAxes() {
	glCallList(AXES_LIST_ID);
}

void RenderManager::pauseSpObjsRendering() {
	isPaused = true;
}

void RenderManager::resumeSpObjsRendering() {
	isPaused = false;
}

bool RenderManager::isRenderingPaused() {
	return isPaused;
}

RenderManager::~RenderManager() {
	for (SpaceObject * spObj : spObjsList) {
		delete spObj;
	}
}