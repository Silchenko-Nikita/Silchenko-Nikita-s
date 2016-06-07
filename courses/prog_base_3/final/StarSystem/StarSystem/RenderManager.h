#pragma once

#include <vector>
#include "SpaceObjects.h"
#include "constants.h"

class SpaceObject;

class RenderManager {
	static RenderManager * instance;
	static const int backgroundStarsNumber;
	GLUquadricObj * quadric;

	std::list<SpaceObject *> spObjsList;
	bool _isPaused = false;
	bool _areAxesDisplaying = true;
	bool _isBackgroundDisplaying = true;

	RenderManager();
	RenderManager(const RenderManager &) {};
	RenderManager& operator=(RenderManager &) {};

public:
	static RenderManager * getInstance();

	void renderSpaceObject(SpaceObject * spObj);

	void initBackground();
	void initSphere();
	void initTeapot();
	void initAxes();
	void initAll();

	void updateSpaceObjects();

	void displaySpaceObjects();
	void displayBackground();
	void displayAxes();
	void displayAll();

	void setAxesDisplaying(bool val);
	bool areAxesDisplaying();

	void setBackgroundDisplaying(bool val);
	bool isBackgroundDisplaying();

	void pauseSpObjsRendering();
	void resumeSpObjsRendering();
	bool isRenderingPaused();

	~RenderManager();
};