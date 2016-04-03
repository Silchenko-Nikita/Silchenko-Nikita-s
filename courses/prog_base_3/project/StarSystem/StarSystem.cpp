// StarSystem.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <GL/freeglut.h> 

#include <AntTweakBar.h>  

#include <stdio.h>  
#include <stdlib.h>

#include "glutCB.h"

const size_t initialWindowWidth = 1200;
const size_t initialWindowHeight = 600;

int main(int argc, char ** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(initialWindowWidth, initialWindowHeight);
	glutCreateWindow("My Window");

	TwInit(TW_OPENGL, NULL);

	glutDisplayFunc(GlutCB::Display);
	glutReshapeFunc(GlutCB::Reshape);
	
	glutMouseFunc(GlutCB::Mouse);
	glutMotionFunc(GlutCB::Motion);
	glutPassiveMotionFunc(GlutCB::PassiveMotion);
	glutKeyboardFunc(GlutCB::Keyboard);
	glutSpecialFunc(GlutCB::Special);

	TwGLUTModifiersFunc(glutGetModifiers);
		
	glutTimerFunc(10, GlutCB::Timer, 1);

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	float something = 0.0f;
	TwBar * bar = TwNewBar("ControlPane");
	TwDefine(" ControlPane size='300 400' color='100 200 200' ");
	TwAddVarRW(bar, "Some sample changeble val", TW_TYPE_FLOAT, &something, " min=-100.0 max=100.0 step=0.5 ");

	glutMainLoop();
	
	TwTerminate();
	return EXIT_SUCCESS;
}