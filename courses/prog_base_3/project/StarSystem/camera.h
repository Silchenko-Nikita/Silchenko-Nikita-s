#pragma once

#include "vector3.h"

namespace Camera {
	void init();
	void rotate(double angle, double anglev);
	void move(double delta);
}