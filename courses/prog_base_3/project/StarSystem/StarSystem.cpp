// StarSystem.cpp : Defines the entry point for the console application.
//

#define _CRT_SECURE_NO_DEPRECATE

#include "stdafx.h"

#include <GL/freeglut.h> 

#include <stdio.h>
#include <stdlib.h>

#include "glutCB.h"
#include "RenderManager.h"
#include "constants.h"
#include "vector3.h"
#include "SpaceObjects.h"
#include "ControlPane.h"

// main func is temporary ugly
int main(int argc, char ** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(glutGet(GLUT_SCREEN_WIDTH) / 14, glutGet(GLUT_SCREEN_HEIGHT) / 21);
	glutInitWindowSize(glutGet(GLUT_SCREEN_WIDTH) * 6 / 7, glutGet(GLUT_SCREEN_HEIGHT) * 6/7);
	int window = glutCreateWindow("Star System");
	//glutFullScreen();

	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_LINE_SMOOTH);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);

	//glCullFace(GL_BACK);
	
	//glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	TwInit(TW_OPENGL, NULL);

	glutDisplayFunc(GlutCB::Display);
	glutReshapeFunc(GlutCB::Reshape);
	
	glutMouseFunc(GlutCB::Mouse);
	glutMotionFunc(GlutCB::Motion);
	glutPassiveMotionFunc(GlutCB::PassiveMotion);
	glutKeyboardFunc(GlutCB::Keyboard);
	glutKeyboardUpFunc(GlutCB::KeyboardUp);
	glutSpecialFunc(GlutCB::Special);
	glutSpecialUpFunc(GlutCB::SpecialUp);

	TwGLUTModifiersFunc(glutGetModifiers);

	glutTimerFunc(Constants::deltaTime, GlutCB::Timer, 1);
	
	ControlPane * ctrlPane = ControlPane::getInstance();

	Vector3f color(1.0f, 1.0f, 0.0f);
	Vector3d velocity(0.0, 0.0, 0.0);
	Vector3d pos(0.0, 0.0, 0.0);
	Star sun(GL_LIGHT0, "Sun", 2.0e30, 1.4e9, color, pos, velocity);
	ctrlPane->addSpObj(&sun);
	RenderManager::addSpaceObjectForRendering(&sun);


	Vector3f color1(0.0f, 0.0f, 1.0f);
	Vector3d velocity1(0.0, 3.0e4, 0.0);
	Vector3d pos1(1.5e11, 0.0, 0.0);
	Planet earth("Earth", 6.0e24, 1.2e7, color1, pos1, velocity1);
	ctrlPane->addSpObj(&earth);
	RenderManager::addSpaceObjectForRendering(&earth);

	Vector3f color2(0.7f, 0.7f, 0.7f);
	Vector3d velocity2(0.0, 1.0e3, 0.0);
	Vector3d pos2(4.0e8, 0.0, 0.0);
	Sputnik moon("Moon", 7.35e22, 3.4e6, color2, &earth, pos2, velocity2);
	ctrlPane->addSpObj(&moon);
	RenderManager::addSpaceObjectForRendering(&moon);
	
	Vector3f color3(1.0f, 0.0f, 0.0f);
	Vector3d velocity3(0.0, 1.0e4, 0.0);
	Vector3d pos3(5.0e11, 0.0, 0.0);
	Planet tralfamadore("Tralfamadore", 6.0e24, 1.2e7, color3, pos3, velocity3);
	ctrlPane->addSpObj(&tralfamadore);
	RenderManager::addSpaceObjectForRendering(&tralfamadore);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glutMainLoop();
	
	delete ctrlPane;
	glutDestroyWindow(window);
	return EXIT_SUCCESS;
}