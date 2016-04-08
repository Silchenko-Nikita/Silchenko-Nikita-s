#include "stdafx.h"

#include <GL/freeglut.h> 
#include <AntTweakBar.h>
#include <iostream>

#include "glutCB.h"
#include "manager.h"
#include "constants.h"
#include "camera.h"

static constexpr double operator "" _rads(long double dgr) {
	return dgr / 180.0 * CONST_PI;
}

namespace GlutCB {
	static double cameraDist = 700.0;

	void Display()
	{

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		Manager::displaySpaceObjects();
		
		TwDraw();

		glutSwapBuffers();
	}

	void Reshape(int w, int h) {
		if (h == 0) h = 1;
		glViewport(0, 0, w, h);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(60, ((float)w) / ((float)h), 10, 1000);
		glTranslated(0, 0, -cameraDist);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		
		TwWindowSize(w, h);
	}

	void Mouse(int button, int state, int x, int y) {
		TwEventMouseButtonGLUT(button, state, x, y);

		glMatrixMode(GL_PROJECTION);
		if (button == 3) { // scroll up
			glTranslatef(0.0f, 0.0f, 5.0f);
		}
		else if (button == 4) { // scroll down
			glTranslatef(0.0f, 0.0f, -5.0f);
		}
		glMatrixMode(GL_MODELVIEW);
	}

	void Motion(int x, int y) {
		TwEventMouseMotionGLUT(x, y);
	}

	void PassiveMotion(int x, int y) {
		TwEventMouseMotionGLUT(x, y);
	}

	void Keyboard(unsigned char key, int x, int y) {
		TwEventKeyboardGLUT(key, x, y);

		glMatrixMode(GL_PROJECTION);
		switch (key)
		{
		/*case 'w':
		case 'W':
			Camera::rotateUp(1.0);
			break;
		case 'a':
		case 'A':
			Camera::rotateRight(1.0);
			break;
		case 'd':
		case 'D':
			Camera::rotateRight(-1.0);
			break;
		case 's':
		case 'S':
			Camera::rotateUp(-1.0);
			break;*/

		case 'f':
		case 'F':
			glutFullScreen();
			break;
		case 27: //escape
			glutPositionWindow(glutGet(GLUT_SCREEN_WIDTH) / 14, glutGet(GLUT_SCREEN_HEIGHT) / 28);
			glutReshapeWindow(glutGet(GLUT_SCREEN_WIDTH) * 6 / 7, glutGet(GLUT_SCREEN_HEIGHT) * 6 / 7);
			break;
		}
		glMatrixMode(GL_MODELVIEW);
	}

	void Special(int key, int x, int y) {
		TwEventSpecialGLUT(key, x, y);
		glMatrixMode(GL_PROJECTION);
		switch (key) {
		case GLUT_KEY_UP:
			glTranslatef(0.0f, 0.0f, 5.0f);
			break;
		case GLUT_KEY_DOWN:
			glTranslatef(0.0f, 0.0f, -5.0f);
			break;
		}
		glMatrixMode(GL_MODELVIEW);
	}

	void Timer(int v) {
		Manager::updateSpaceObjects();

		glutPostRedisplay();
		glutTimerFunc(Constants::deltaTime, Timer, v);
	}
}