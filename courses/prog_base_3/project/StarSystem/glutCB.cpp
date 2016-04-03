#include "stdafx.h"

#include <GL/freeglut.h> 
#include <AntTweakBar.h>
#include <iostream>

#include "glutCB.h"


namespace GlutCB {
	void Display()
	{
		static int callsCount = 0;
		callsCount++;
		if (callsCount*0.2f > 360) callsCount = 0;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glPushMatrix();
		glColor3f(1.0f, 1.0f, 0.0f);
		glutSolidSphere(25.0, 360, 360);
		glRotatef(callsCount*0.2f, 0.0f, 0.0f, 1.0f);
		glTranslatef(200.0f, 0.0f, 0.0f);
		glColor3f(0.0f, 0.0f, 1.0f);
		glutSolidSphere(10.0, 360, 360);
		glRotatef(callsCount*2.0f, 0.0f, 0.0f, 1.0f);
		glTranslatef(15.0f, 0.0f, 0.0f);
		glColor3f(2.0f, 8.0f, 8.0f);
		glutSolidSphere(2.0, 360, 360);
		glPopMatrix();

		TwDraw();

		glutSwapBuffers();
	}

	void Reshape(int w, int h) {
		glViewport(0, 0, w, h);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0.0, w, 0.0, h, -1000.0, 1000.0);
		glTranslatef(w/2, h/2, 1000.0f);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		
		TwWindowSize(w, h);
	}

	void Mouse(int button, int state, int x, int y) {
		TwEventMouseButtonGLUT(button, state, x, y);
	}

	void Motion(int x, int y) {
		TwEventMouseMotionGLUT(x, y);
	}

	void PassiveMotion(int x, int y) {
		TwEventMouseMotionGLUT(x, y);
	}

	void Keyboard(unsigned char key, int x, int y) {
		TwEventKeyboardGLUT(key, x, y);
	}

	void Special(int key, int x, int y) {
		TwEventSpecialGLUT(key, x, y);
	}

	void Timer(int v) {
		glutPostRedisplay();
		glutTimerFunc(10, Timer, v);
	}
}