#include "stdafx.h"

#include <GL/freeglut.h> 
#include <AntTweakBar.h>
#include <iostream>

#include "glutCB.h"
#include "RenderManager.h"
#include "constants.h"
#include "SpaceObjects.h"
#include "camera.h"

static Camera * _camera;
static RenderManager * _renderManager;

namespace GlutCBInitializer {
	void init(Camera * camera, RenderManager * renderManager) {
		_camera = camera;
		_renderManager = renderManager;
	}
}

namespace GlutCB {

	void Display()
	{
		/*
		static int sec = 0;
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
		*/
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		_renderManager->displayBackground();
		_renderManager->displayAxes();
		_renderManager->displaySpaceObjects();
	
		TwDraw();

		glutSwapBuffers();
	}

	void Reshape(int w, int h) {
		if (h == 0) h = 1;
		glViewport(0, 0, w, h);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(60, ((float)w) / ((float)h), 0.5f, 2*Camera::maxDistance);

		TwWindowSize(w, h);
	}

	void Mouse(int button, int state, int x, int y) {
		TwEventMouseButtonGLUT(button, state, x, y);

		if (button == 3) { // scroll up
			_camera->zoom(-0.01f);
		}
		else if (button == 4) { // scroll down
			_camera->zoom(0.01f);
		}
	}

	void Motion(int x, int y) {
		TwEventMouseMotionGLUT(x, y);		
	}

	void PassiveMotion(int x, int y) {
		TwEventMouseMotionGLUT(x, y);
	}


	void Keyboard(unsigned char key, int x, int y) {
		TwEventKeyboardGLUT(key, x, y);
		switch (key)
		{

		case 'p':
		case 'P':
			_renderManager->pauseSpObjsRendering();
			break;
		case 'r':
		case 'R':
			_renderManager->resumeSpObjsRendering();
			break;
		case 'a':
		case 'A':
			_renderManager->setAxesDisplaying(!_renderManager->areAxesDisplaying());
			break;
		case 's':
		case 'S':
			_renderManager->setBackgroundDisplaying(!_renderManager->isBackgroundDisplaying());
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
	}

	void KeyboardUp(unsigned char key, int x, int y) {
		//Camera::rotate(0.0, 0.0);
	}


	void Special(int key, int x, int y) {
		TwEventSpecialGLUT(key, x, y);

		switch (key) {
		case GLUT_KEY_UP:
			_camera->zoom(-0.01f);
			break;
		case GLUT_KEY_LEFT:
			break;
		case GLUT_KEY_RIGHT:
				break;
		case GLUT_KEY_DOWN:
			_camera->zoom(0.01f);
			break;
		}
	}

	void SpecialUp(int key, int x, int y) {
		switch (key) {
		case GLUT_KEY_UP:
			//Camera::move(0);
			break;
		case GLUT_KEY_LEFT:
			break;
		case GLUT_KEY_RIGHT:
			break;
		case GLUT_KEY_DOWN:
			//Camera::move(0);
			break;
		}
	}

	void Timer(int v) {
		_camera->render();
		_renderManager->updateSpaceObjects();

		glutPostRedisplay();
		glutTimerFunc(Constants::deltaTime, Timer, v);
	}
}