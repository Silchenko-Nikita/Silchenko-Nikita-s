#include "stdafx.h"

#include <GL/freeglut.h> 
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "SpaceObjects.h"
#include "constants.h"


SpaceObject::SpaceObject(double mass, double diameter, Vector3d & color, Vector3d & position, Vector3d & speed)
{
	this->mass = mass;
	this->diameter = diameter;
	this->color = color;
	this->position = position;
	this->speed = speed;

}

void SpaceObject::interactWith(SpaceObject * spObj) {
	Vector3d accel;
	double dist = sqrt(pow(spObj->position.x - position.x, 2) + pow(spObj->position.y - position.y, 2) + pow(spObj->position.z - position.z, 2));

	accel.x = Constants::G * (spObj->mass * (spObj->position.x - position.x) / pow(dist, 3));
	accel.y = Constants::G * (spObj->mass * (spObj->position.y - position.y) / pow(dist, 3));
	accel.z = Constants::G * (spObj->mass * (spObj->position.z - position.z) / pow(dist, 3));
	speed += accel * (Constants::deltaTime * 1.0e-3 * Constants::timeAccel);
	position += speed * (Constants::deltaTime * 1.0e-3  * Constants::timeAccel);
}

void Star::update(std::vector<SpaceObject *> spObjs)
{
	for (SpaceObject * spObj : spObjs) {
		if ((spObj == this) || (spObj->getType() == PLANET) || (spObj->getType() == SPUTNIK)) continue;

		interactWith(spObj);
	}
}

void Planet::update(std::vector<SpaceObject *> spObjs)
{
	for (SpaceObject * spObj : spObjs) {
		if ((spObj == this) || (spObj->getType() == SPUTNIK)) continue;

		interactWith(spObj);
	}
}

void Sputnik::update(std::vector<SpaceObject *> spObjs)
{
	for (SpaceObject * spObj : spObjs) {
		if (spObj == this) continue;

		interactWith(spObj);
	}
}

void Star::display() {
	glLoadIdentity();

	glTranslated(position.x / Constants::metersPerPixel, position.y / Constants::metersPerPixel, position.z / Constants::metersPerPixel);

	glColor3d(color.x, color.y, color.z);

	glDisable(GL_LIGHTING);
	glutSolidSphere(Constants::starsDisplayFactor * diameter / Constants::metersPerPixel, 180, 180);
	glEnable(GL_LIGHTING);
	GLfloat lightPos[] = { position.x, position.y, position.z };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
}

void Planet::display() {
	glLoadIdentity();

	glTranslated(position.x / Constants::metersPerPixel, position.y / Constants::metersPerPixel, position.z / Constants::metersPerPixel);

	glColor3d(color.x, color.y, color.z);
	glutSolidSphere(Constants::planetsDisplayFactor * diameter / Constants::metersPerPixel, 180, 180);
}

void Sputnik::display() {
	glLoadIdentity();

	glTranslated(position.x / Constants::metersPerPixel, position.y / Constants::metersPerPixel, position.z / Constants::metersPerPixel);

	glColor3d(color.x, color.y, color.z);
	glutSolidSphere(Constants::sputniksDisplayFactor * diameter / Constants::metersPerPixel, 180, 180);
}

SpaceObjectTypes Star::getType() {
	return STAR;
}

SpaceObjectTypes Planet::getType() {
	return PLANET;
}

SpaceObjectTypes Sputnik::getType() {
	return SPUTNIK;
}


void SpaceObject::associateWithTwBar(TwBar * bar){
	//TwAddVarRW(bar, "mass", TW_TYPE_DOUBLE, &mass, " min=1.0e29 max=1.0e33 step=1.0e29");
}