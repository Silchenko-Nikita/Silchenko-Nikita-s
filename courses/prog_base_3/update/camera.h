#pragma once

#include "vector3.h"


class Camera {
	float dist;
	float quat[4];
	Vector3f pos;

public:
	static const float maxDistance;

	Camera();

	void init(); // for avoid binding init to constructor
	void render();
	void move(float delta);
	void setTo(Vector3d & pos);

	friend class ControlPane;
};