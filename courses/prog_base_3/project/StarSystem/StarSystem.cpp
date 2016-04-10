// StarSystem.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <GL/freeglut.h> 

#include <AntTweakBar.h>  

#include <stdio.h>
#include <stdlib.h>

#include "glutCB.h"
#include "manager.h"
#include "constants.h"
#include "vector3d.h"
#include "SpaceObjects.h"

// main func is temporary ugly
int main(int argc, char ** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(glutGet(GLUT_SCREEN_WIDTH) / 14, glutGet(GLUT_SCREEN_HEIGHT) / 21);
	glutInitWindowSize(glutGet(GLUT_SCREEN_WIDTH) * 6 / 7, glutGet(GLUT_SCREEN_HEIGHT) * 6/7);
	int window = glutCreateWindow("My Window");
	//glutFullScreen();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	//glEnable(GL_COLOR_MATERIAL);
	//glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	float lightAmb[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);

	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	TwInit(TW_OPENGL, NULL);

	glutDisplayFunc(GlutCB::Display);
	glutReshapeFunc(GlutCB::Reshape);
	
	glutMouseFunc(GlutCB::Mouse);
	glutMotionFunc(GlutCB::Motion);
	glutPassiveMotionFunc(GlutCB::PassiveMotion);
	glutKeyboardFunc(GlutCB::Keyboard);
	glutSpecialFunc(GlutCB::Special);

	TwGLUTModifiersFunc(glutGetModifiers);

	glutTimerFunc(Constants::deltaTime, GlutCB::Timer, 1);
		
	TwDefine(" GLOBAL contained=true ");
	TwBar * bar = TwNewBar("ControlPane");
	TwDefine(" ControlPane size='300 400' color='100 200 200' ");

	Vector3d color(1.0, 1.0, 0.0);
	Vector3d speed(0.0, 0.0, 0.0);
	Vector3d pos(0.0, 0.0, 0.0);
	Star sun(2.0e30, 1.4e9, color, pos, speed);
	Manager::addSpaceObjectForRendering(&sun);

	Vector3d color1(0.0, 0.0, 1.0);
	Vector3d speed1(0.0, 3.0e4, 0.0);
	Vector3d pos1(1.5e11, 0.0, 0.0);
	Planet earth(6.0e24, 1.2e7, color1, pos1, speed1);
	earth.associateWithTwBar(bar);
	Manager::addSpaceObjectForRendering(&earth);

	/*Vector3d color2(0.7, 0.7, 0.7);
	Vector3d speed2(0.0, 3.1e4, 0.0);
	Vector3d pos2(1.504e11, 0.0, 0.0);
	Sputnik moon(7.35e22, 3.4e6, color2, pos2, speed2);
	Manager::addSpaceObjectForRendering(&moon);*/
	
	Vector3d color3(1.0, 0.0, 0.0);
	Vector3d speed3(0.0, 1.0e4, 0.0);
	Vector3d pos3(5.0e11, 0.0, 0.0);
	Planet tralfamadore(6.0e24, 1.2e7, color3, pos3, speed3);
	Manager::addSpaceObjectForRendering(&tralfamadore);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glutMainLoop();
	
	TwTerminate();
	glutDestroyWindow(window);
	return EXIT_SUCCESS;
}