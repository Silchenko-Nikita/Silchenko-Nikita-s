#include "stdafx.h"

#include <GL/freeglut.h> 
#include <AntTweakBar.h>
#include <iostream>

#include "glutCB.h"
#include "RenderManager.h"
#include "constants.h"
#include "camera.h"

namespace GlutCB {
	void Display()
	{
		/*static int sec = 0;
		static int ml = glutGet(GLUT_ELAPSED_TIME);
		static int fcounter = 0;

		ml = glutGet(GLUT_ELAPSED_TIME) - 1000* sec;
		fcounter++;
		if (ml > 1000*(sec + 1)) {
			fcounter = 0;
			ml = 0;
			sec++;
		}
		printf("%d %d\n", fcounter, sec);
		*/ //for debug

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		RenderManager::displaySpaceObjects();
		
		TwDraw();

		glutSwapBuffers();
	}

	void Reshape(int w, int h) {
		if (h == 0) h = 1;
		glViewport(0, 0, w, h);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(60, ((float)w) / ((float)h), 1, 2000);
		Camera::init();
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		
		TwWindowSize(w, h);
	}

	void Mouse(int button, int state, int x, int y) {
		TwEventMouseButtonGLUT(button, state, x, y);

		glMatrixMode(GL_PROJECTION);
		if (button == 3) { // scroll up
			Camera::move(5.0);
		}
		else if (button == 4) { // scroll down
			Camera::move(-5.0);
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
		case 'w':
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
			break;

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
			Camera::move(5.0);
			break;
		case GLUT_KEY_DOWN:
			Camera::move(-5.0);
			break;
		}
		glMatrixMode(GL_MODELVIEW);
	}

	void Timer(int v) {
		RenderManager::updateSpaceObjects();

		glutPostRedisplay();
		glutTimerFunc(Constants::deltaTime, Timer, v);
	}
}