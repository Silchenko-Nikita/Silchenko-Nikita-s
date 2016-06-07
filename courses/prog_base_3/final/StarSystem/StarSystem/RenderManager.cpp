#include "stdafx.h"

#include "utils.h"
#include "Camera.h"
#include "Vector3.h"
#include "RenderManager.h"


RenderManager * RenderManager::instance = NULL;
const int RenderManager::backgroundStarsNumber = 256;


RenderManager * RenderManager::getInstance()
{
	if (NULL != instance) return NULL;

	instance = new RenderManager();
	
	return instance;
}

RenderManager::RenderManager() {
	quadric = gluNewQuadric();
	gluQuadricNormals(quadric, GLU_SMOOTH);
	gluQuadricDrawStyle(quadric, GLU_FILL);
	gluQuadricTexture(quadric, GL_TRUE);
}

void RenderManager::renderSpaceObject(SpaceObject * spObj) {
	spObjsList.push_back(spObj);
}

void RenderManager::initSphere() {
	glNewList(SPHERE_LIST_ID, GL_COMPILE);

	glutSolidSphere(1.0, 48, 48);

	glEndList();
}

void RenderManager::initTeapot() {
	glNewList(TEAPOT_LIST_ID, GL_COMPILE);

	glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	glutSolidTeapot(1.0);

	glEndList();
}

void RenderManager::initBackground() {
	glNewList(BACKGROUND_LIST_ID, GL_COMPILE);

	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);
	glColor3f(1.0f, 1.0f, 1.0f);

	glBegin(GL_POINTS);
	for (int i = 0; i < backgroundStarsNumber; i++)
	{
		Vector3f starPos = Vector3f(Utils::randf(), Utils::randf(), Utils::randf()).normalized() * 2.0 * Camera::maxDistance;
		glVertex3f(starPos.x, starPos.y, starPos.z);
	}
	glEnd();

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);

	glEndList();
}

void RenderManager::initAxes() {
	glNewList(AXES_LIST_ID, GL_COMPILE);

	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_LINE_STIPPLE);
	glColor3f(1.0f, 1.0f, 1.0f);
	glLineStipple(10, 0x5555);

	glBegin(GL_LINES);
		glVertex3f(-2.0f*Camera::maxDistance, 0.0f, 0.0f);
		glVertex3f(2.0f*Camera::maxDistance, 0.0f, 0.0f);
		glVertex3f(0.0f, -2.0f*Camera::maxDistance, 0.0f);
		glVertex3f(0.0f, 2.0f*Camera::maxDistance, 0.0f);
		glVertex3f(0.0f, 0.0f, -2.0f*Camera::maxDistance);
		glVertex3f(0.0f, 0.0f, 2.0f*Camera::maxDistance);
	glEnd();

	glDisable(GL_LINE_STIPPLE);
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);

	glEndList();
}


void RenderManager::initAll() {
	initSphere();
	initTeapot();
	initBackground();
	initAxes();
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
			spObj->display(quadric);
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

void RenderManager::displayAll()
{
	displayBackground();
	displayAxes();
	displaySpaceObjects();
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
	gluDeleteQuadric(quadric);
}