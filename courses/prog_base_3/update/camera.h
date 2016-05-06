#pragma once

#include "vector3.h"


namespace Camera {
	extern float quat[];
	void render();
	void move(float delta);
	void setTo(Vector3d & pos);
}