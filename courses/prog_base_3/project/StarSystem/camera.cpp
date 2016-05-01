#include "stdafx.h"

#include "math.h"
#include <math.h>
#include <GL/freeglut.h> 

#include "utils.h"
#include "camera.h"
#include "constants.h"


namespace Camera {
	double dist = 700.0;
	Vector3d up(0.0, 1.0, 0.0);
	Vector3d right(1.0, 0.0, 0.0);

	void init() {
		glTranslated(0.0, 0.0, -dist);
	}

	void move(double delta) {
		dist += delta;
		glTranslated(0.0, 0.0, delta);
	}

	void rotateRight(double angle) {
		glMatrixMode(GL_PROJECTION);
		glRotated(angle, 0.0, 1.0, 0.0);
		glMatrixMode(GL_MODELVIEW);

		/*glMatrixMode(GL_PROJECTION);
		glRotated(angle, up.x, up.y, up.z);
		glMatrixMode(GL_MODELVIEW);

		double a = (cos(Utils::toRads(angle))*up.x) / (right.y*up.x - up.y*right.x);
		double b = - (right.z*up.x - up.z*right.x) / (right.y*up.x - up.y*right.x);
		double c = - a*up.y / up.x;
		double d = - (b*up.y + up.z) / up.x;

		double D = pow((a*b + c*d), 2) - (b*b + d*d + 1)*(a*a + c*c - 1);
		
		right.z = (-(a*b + c*d) + sqrt(D)) / (b*b + d*d + 1);
		right.y = a + b*right.z;
		right.x = c + d*right.z;*/
	}

	void rotateUp(double angle) {
		glMatrixMode(GL_PROJECTION);
		glRotated(angle, 1.0, 0.0, 0.0);
		glMatrixMode(GL_MODELVIEW);

		/*glMatrixMode(GL_PROJECTION);
		glRotated(angle, right.x, right.y, right.z);
		glMatrixMode(GL_MODELVIEW);

		double a = (cos(Utils::toRads(angle))*right.x) / (up.y*right.x - right.y*up.x);
		double b = -(up.z*right.x - right.z*up.x) / (up.y*right.x - right.y*up.x);
		double c = -a*right.y / right.x;
		double d = -(b*right.y + right.z) / right.x;

		double D = pow((a*b + c*d), 2) - (b*b + d*d + 1)*(a*a + c*c - 1);

		up.z = (-(a*b + c*d) + sqrt(D)) / (b*b + d*d + 1);
		up.y = a + b*up.z;
		up.x = c + d*up.z;*/
	}
}