#include "stdafx.h"

#include "utils.h"
#include "Camera.h"
#include "Vector3.h"
#include "RenderManager.h"


RenderManager * RenderManager::instance = NULL;
const int RenderManager::backgroundStarsNumber = 256;

RenderManager::RenderManager() {
	_isPaused = false;
	_areAxesDisplaying = true;
	_isBackgroundDisplaying = true;
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
	for (int i = 0; i < backgroundStarsNumber; i++)
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
		glVertex3f(-Camera::maxDistance, 0.0f, 0.0f);
		glVertex3f(Camera::maxDistance, 0.0f, 0.0f);
		glVertex3f(0.0f, -Camera::maxDistance, 0.0f);
		glVertex3f(0.0f, Camera::maxDistance, 0.0f);
		glVertex3f(0.0f, 0.0f, -Camera::maxDistance);
		glVertex3f(0.0f, 0.0f, Camera::maxDistance);
	glEnd();

	glDisable(GL_LINE_STIPPLE);
	glEnable(GL_LIGHTING);

	glEndList();
}


void RenderManager::updateSpaceObjects() {
	if (_isPaused) return;
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
	if (_isBackgroundDisplaying) {
		glCallList(BACKGROUND_LIST_ID);
	}
}

void RenderManager::displayAxes() {
	if (_areAxesDisplaying) {
		glCallList(AXES_LIST_ID);
	}
}

void RenderManager::pauseSpObjsRendering() {
	_isPaused = true;
}

void RenderManager::resumeSpObjsRendering() {
	_isPaused = false;
}

void RenderManager::setAxesDisplaying(bool val) {
	_areAxesDisplaying = val;
}

bool RenderManager::areAxesDisplaying()
{
	return _areAxesDisplaying;
}

void RenderManager::setBackgroundDisplaying(bool val) {
	_isBackgroundDisplaying = val;
}

bool RenderManager::isBackgroundDisplaying()
{
	return _isBackgroundDisplaying;
}

bool RenderManager::isRenderingPaused() {
	return _isPaused;
}

RenderManager::~RenderManager() {
	for (SpaceObject * spObj : spObjsList) {
		delete spObj;
	}
}