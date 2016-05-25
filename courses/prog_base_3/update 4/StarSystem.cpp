// StarSystem.cpp : Defines the entry point for the console application.
//

#define _CRT_SECURE_NO_DEPRECATE

#include "stdafx.h"

#include <GL/freeglut.h> 

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "glutCB.h"
#include "RenderManager.h"
#include "constants.h"
#include "vector3.h"
#include "Camera.h"
#include "SpaceObjects.h"
#include "ControlPane.h"

void TW_CALL CopyStdStringToClient(std::string& destinationClientString, const std::string& sourceLibraryString)
{
	destinationClientString = sourceLibraryString;
}

// main func is temporary ugly
int main(int argc, char ** argv)
{
	srand(time(NULL));

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE | GLUT_ACCUM);
	glutInitWindowPosition(glutGet(GLUT_SCREEN_WIDTH) / 14, glutGet(GLUT_SCREEN_HEIGHT) / 21);
	glutInitWindowSize(glutGet(GLUT_SCREEN_WIDTH) * 6 / 7, glutGet(GLUT_SCREEN_HEIGHT) * 6/7);
	int window = glutCreateWindow("Star System");
	//glutFullScreen();

	glEnable(GL_ACCUM);
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

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	Star sun("Sun", 2.0e30, 1.4e9, Vector3f(1.0f, 1.0f, 0.0f), Vector3d(0.0, 0.0, 0.0), Vector3d(0.0, 0.0, 0.0));

	Planet mercury("Mercury", 3.3e23, 5.0e6, Vector3f(0.8f, 0.0f, 0.0f), Vector3d(4.6e10, 0.0, 0.0), Vector3d(0.0, 4.7e4, 0.0));

	Planet venus("Venus", 4.8e24, 1.2e7, Vector3f(0.8f, 0.6f, 0.7f), Vector3d(1.0e11, 0.0, 0.0), Vector3d(0.0, 3.5e4, 0.0));

	Planet earth("Earth", 6.0e24, 1.2e7, Vector3f(0.0f, 0.0f, 1.0f), Vector3d(1.5e11, 0.0, 0.0), Vector3d(0.0, 3.0e4, 0.0));
	Sputnik moon("Moon", 7.35e22, 3.4e6, Vector3f(0.7f, 0.7f, 0.7f), &earth, Vector3d(4.0e8, 0.0, 0.0), Vector3d(0.0, 1.0e3, 0.0));

	Planet mars("Mars", 6.4e23, 6.7e6, Vector3f(1.0f, 0.0f, 0.0f), Vector3d(2.0e11, 0.0, 0.0), Vector3d(0.0, 2.4e4, 0.0));

	Teapot rasselsTeapot("RasselsTeapot", 0.0, 0.3, Vector3f(1.0f, 1.0f, 1.0f), Vector3d(3.0e11, 0.0, 0.0), Vector3d(0.0, 2.0e4, 0.0));

	Planet jupiter("Jupiter", 1.9e27, 1.3e8, Vector3f(1.0f, 0.8f, 0.0f), Vector3d(7.4e11, 0.0, 0.0), Vector3d(0.0, 1.3e4, 0.0));

	RenderManager * renderManager = RenderManager::getInstance();
	renderManager->initAll();

	Camera * camera = new Camera();
	
	ControlPane * ctrlPane = ControlPane::getInstance(camera, renderManager);
	ctrlPane->show();

	TwCopyStdStringToClientFunc(CopyStdStringToClient);

	ctrlPane->addSpaceObject(&sun);
	renderManager->renderSpaceObject(&sun);

	ctrlPane->addSpaceObject(&mercury);
	renderManager->renderSpaceObject(&mercury);

	ctrlPane->addSpaceObject(&venus);
	renderManager->renderSpaceObject(&venus);

	ctrlPane->addSpaceObject(&earth);
	renderManager->renderSpaceObject(&earth);

	ctrlPane->addSpaceObject(&moon);
	renderManager->renderSpaceObject(&moon);

	ctrlPane->addSpaceObject(&mars);
	renderManager->renderSpaceObject(&mars);

	ctrlPane->addSpaceObject(&rasselsTeapot);
	renderManager->renderSpaceObject(&rasselsTeapot);

	ctrlPane->addSpaceObject(&jupiter);
	renderManager->renderSpaceObject(&jupiter);

	GlutCBInitializer::init(camera, renderManager);

	glutMainLoop();
	
	delete ctrlPane;
	glutDestroyWindow(window);
	return EXIT_SUCCESS;
}