#include "stdafx.h"

#include <math.h>
#include <GL/freeglut.h> 
#include <iostream>

#include "utils.h"
#include "camera.h"
#include "constants.h"

static double angle, anglev;
static double deltaAngle, deltaAnglev, deltaMove;
static Vector3d v, l, up;
static double deltaSpeed;
static double speed;

namespace Camera {
	static void _render();

	void init() {
		angle = 0.0;
		anglev = 0.0;

		deltaAngle = 0.0;
		deltaAnglev = 0.0;
		deltaMove = 0.0;

		v.x = 0.0;
		v.y = 0.0;
		v.z = 600.0;

		l.x = 0.0;
		l.y = 0.0;
		l.z = -600.0;

		up.x = 0.0;
		up.y = 1.0;
		up.z = 0.0f;

		speed = 10.0f;
		deltaSpeed = 0.0f;

		_render();
	}

	static void _render()
	{
		glLoadIdentity();
		
		speed += deltaSpeed;
		if (speed < 0.0f) speed = 0.0f;

		v.x += deltaMove * l.x * speed;
		v.y += deltaMove * l.y * speed;
		v.z += deltaMove * l.z * speed;

		angle += deltaAngle;
		anglev += deltaAnglev;

		l.x = sin(angle) * cos(anglev);
		l.y = sin(anglev);
		l.z = -cos(angle) * cos(anglev);

		if (cos(anglev) < 0.0)
		{
			up.y = -1.0;
		}
		else
		{
			up.y = 1.0;
		}

		gluLookAt(v.x,  v.y, v.z,
			v.x + l.x, v.y + l.y, v.z + l.z,
			up.x, up.y, up.z);
	}

	void rotate(double angle, double anglev) {
		deltaAngle = angle;
		deltaAnglev = anglev;

		_render();
	}

	void move(double delta) {
		deltaMove = delta;

		_render();
	}
}