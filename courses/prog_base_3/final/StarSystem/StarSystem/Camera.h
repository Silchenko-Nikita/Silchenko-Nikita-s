#pragma once

#include "vector3.h"
#include "SpaceObjects.h"

class Camera {
	float dist;
	float quat[4];
	Vector3f pos;
	SpaceObject * targetSpObj = NULL;

public:
	static const float maxDistance;

	Camera(const float initialDistance);

	void init(const float initialDistance); // for avoid binding init to constructor
	void render();
	void zoom(float deltaP);
	void setTarget(SpaceObject * spObj);

	friend class ControlPane;
};
