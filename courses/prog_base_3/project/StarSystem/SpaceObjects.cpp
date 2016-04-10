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
	double dist = sqrt(pow(spObj->position.x - position.x, 2) + pow(spObj->position.y - position.y, 2) + pow(spObj->position.z - position.z, 2));

	accel.x += Constants::G * (spObj->mass * (spObj->position.x - position.x) / pow(dist, 3));
	accel.y += Constants::G * (spObj->mass * (spObj->position.y - position.y) / pow(dist, 3));
	accel.z += Constants::G * (spObj->mass * (spObj->position.z - position.z) / pow(dist, 3));
	
	speed += accel * (Constants::deltaTime * 1.0e-3 * Constants::timeAccel);
	position += speed * (Constants::deltaTime * 1.0e-3  * Constants::timeAccel);
}

void Star::update(std::vector<SpaceObject *> spObjs)
{
	accel.nullify();
	for (SpaceObject * spObj : spObjs) {
		if ((spObj == this) || (spObj->getType() == PLANET) || (spObj->getType() == SPUTNIK)) continue;

		interactWith(spObj);
	}
}

void Planet::update(std::vector<SpaceObject *> spObjs)
{
	accel.nullify();
	for (SpaceObject * spObj : spObjs) {
		if ((spObj == this) || (spObj->getType() == SPUTNIK)) continue;

		interactWith(spObj);
	}
}

void Sputnik::update(std::vector<SpaceObject *> spObjs)
{
	accel.nullify();
	for (SpaceObject * spObj : spObjs) {
		if (spObj == this) continue;

		interactWith(spObj);
	}
}

void Star::display() {
	glLoadIdentity();

	glTranslated(position.x / Constants::metersPerPixel, position.y / Constants::metersPerPixel, position.z / Constants::metersPerPixel);

	float lightPos[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float lightDiff[] = { 0.75f + color.x * 0.25f, 0.75f + color.y * 0.25f, 0.75f + color.z * 0.25f, 1.0f };
	
	glLightfv(GL_LIGHT0 /* + starIndex */, GL_POSITION, lightPos);
	glLightfv(GL_LIGHT0 /* + starIndex */, GL_DIFFUSE, lightDiff);
	
	float materialAmb[] = { color.x, color.y, color.z, 1.0f };
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmb);

	glutSolidSphere(Constants::starsDisplayFactor * diameter / Constants::metersPerPixel, 180, 180);

}

void Planet::display() {

	glLoadIdentity();

	glTranslated(position.x / Constants::metersPerPixel, position.y / Constants::metersPerPixel, position.z / Constants::metersPerPixel);
	
	float materialAmb[] = { color.x * 0.05f, color.y * 0.05f, color.z * 0.05f, 1.0f };
	float materialDiff[] = { 0.2f + color.x*0.2f, 0.2f + color.y*0.2f, 0.2f + color.z*0.2f, 1.0f };
	
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmb);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiff);

	glutSolidSphere(Constants::planetsDisplayFactor * diameter / Constants::metersPerPixel, 180, 180);
}

void Sputnik::display() {
	glLoadIdentity();

	glTranslated(position.x / Constants::metersPerPixel, position.y / Constants::metersPerPixel, position.z / Constants::metersPerPixel);

	float materialAmb[] = { color.x * 0.05f, color.y * 0.05f, color.z * 0.05f, 1.0f };
	float materialDiff[] = { 0.2f + color.x*0.2f, 0.2f + color.y*0.2f, 0.2f + color.z*0.2f, 1.0f };

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmb);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiff);

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