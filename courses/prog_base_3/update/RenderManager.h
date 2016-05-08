#pragma once

#include <vector>
#include "SpaceObjects.h"

class SpaceObject;

static enum DISPLAY_LIST {
	BACKGROUND_LIST_ID = 1,
	AXES_LIST_ID
};

class RenderManager {
	static RenderManager * instance;
	static const int backgroundStarsNumber;

	std::list<SpaceObject *> spObjsList;
	bool isPaused;

	RenderManager();
	RenderManager(const RenderManager &) {};
	RenderManager& operator=(RenderManager &) {};

public:
	static RenderManager * getInstance();

	void renderSpaceObject(SpaceObject * spObj);

	void initBackground();
	void initAxes();
	void updateSpaceObjects();

	void displaySpaceObjects();
	void displayBackground();
	void displayAxes();

	void pauseSpObjsRendering();
	void resumeSpObjsRendering();
	bool isRenderingPaused();

	~RenderManager();
};