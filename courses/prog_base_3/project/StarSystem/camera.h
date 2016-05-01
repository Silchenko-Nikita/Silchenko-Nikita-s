#pragma once

#include "vector3.h"

namespace Camera {
	void init();
	void move(double delta);
	void rotateRight(double angle);
	void rotateUp(double angle);
}