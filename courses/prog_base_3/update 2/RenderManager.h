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
	bool _isPaused;
	bool _areAxesDisplaying;
	bool _isBackgroundDisplaying;

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

	void setAxesDisplaying(bool val);
	bool areAxesDisplaying();

	void setBackgroundDisplaying(bool val);
	bool isBackgroundDisplaying();

	void pauseSpObjsRendering();
	void resumeSpObjsRendering();
	bool isRenderingPaused();

	~RenderManager();
};