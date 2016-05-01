#include "stdafx.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "SpaceObjects.h"
#include "constants.h"
#include "utils.h"


SpaceObject::SpaceObject(const char * name, double mass, double diameter, Vector3f & color, Vector3d & position, Vector3d & velocity)
{
	this->name = name;
	this->mass = mass;
	this->diameter = diameter;
	this->color = color;
	this->position = position;
	this->velocity = velocity;
}

void SpaceObject::interactWith(SpaceObject * spObj) {
	Vector3d absPosition = position;
	if (parent != NULL) absPosition += parent->getPosition();

	Vector3d objPosition = spObj->getPosition();
	double objDiameter = spObj->getDiameter(), objMass = spObj->getMass();
	double dist = sqrt(pow(objPosition.x - absPosition.x, 2) + pow(objPosition.y - absPosition.y, 2) + pow(objPosition.z - absPosition.z, 2));

	if (dist > (diameter/2 + objDiameter/2)) {

		accel.x += Constants::G * (objMass * (objPosition.x - absPosition.x) / pow(dist, 3));
		accel.y += Constants::G * (objMass * (objPosition.y - absPosition.y) / pow(dist, 3));
		accel.z += Constants::G * (objMass * (objPosition.z - absPosition.z) / pow(dist, 3));
	}

}

void SpaceObject::updatePosition() {
	velocity += accel * (Constants::deltaTime * 1.0e-3 * Constants::timeAccel);
	position += velocity * (Constants::deltaTime * 1.0e-3  * Constants::timeAccel);
}

const char * SpaceObject::getName() {
	return name.c_str();
}

double SpaceObject::getMass() {
	return mass;
}

double SpaceObject::getDiameter() {
	return diameter;
}

Vector3d SpaceObject::getAccel() {
	return accel;
}

Vector3d SpaceObject::getVelocity() {
	return velocity;
}

Vector3d SpaceObject::getPosition() {
	return position;
}

SpaceObject * SpaceObject::getParent()
{
	return parent;
}

const char * SpaceObject::getTypeStr() {
	return typeStrRepr(getType());
}

void Planet::addSputnik(Sputnik * sputnik) {
	sputniks.push_back(sputnik);
}

void Star::update(std::list<SpaceObject *> spObjs){
	accel.nullify();
	for (SpaceObject * spObj : spObjs) {
		if ((spObj == this) || (spObj->getType() == PLANET) || (spObj->getType() == SPUTNIK)) continue;

		interactWith(spObj);
	}
	updatePosition();
}

void Planet::update(std::list<SpaceObject *> spObjs){
	accel.nullify();
	for (SpaceObject * spObj : spObjs) {
		if ((spObj == this) || (spObj->getType() == SPUTNIK)) continue;

		interactWith(spObj);
	}
	updatePosition();
}

void Sputnik::update(std::list<SpaceObject *> spObjs){
	accel.nullify();
	for (SpaceObject * spObj : spObjs) {
		if ((spObj == this) || (spObj->getType() == STAR) || ((spObj->getType() == PLANET) && (spObj != parent))) continue;

		interactWith(spObj);
	}
	updatePosition();
}

void Star::display() {
	glLoadIdentity();

	glTranslated(position.x / Constants::metersPerPixel, position.y / Constants::metersPerPixel, position.z / Constants::metersPerPixel);

	float lightPos[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float lightDiff[] = { color.x, color.y, color.z, 1.0f };
	
	glLightfv(glLight, GL_POSITION, lightPos);
	glLightfv(glLight, GL_DIFFUSE, lightDiff);
	
	float materialAmb[] = { color.x, color.y, color.z, 1.0f };
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmb);

	glutSolidSphere(Constants::starsDiamDisplayFactor * diameter / Constants::metersPerPixel, 180, 180);
	glLoadIdentity();
}

void Planet::display() {

	glLoadIdentity();

	glTranslated(position.x / Constants::metersPerPixel, position.y / Constants::metersPerPixel, position.z / Constants::metersPerPixel);
	
	float materialAmb[] = { color.x * 0.05f, color.y * 0.05f, color.z * 0.05f, 1.0f };
	float materialDiff[] = { 0.4f + color.x*0.2f, 0.4f + color.y*0.2f, 0.4f + color.z*0.2f, 1.0f };
	
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmb);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiff);

	glutSolidSphere(Constants::planetsDiamDisplayFactor * diameter / Constants::metersPerPixel, 180, 180);
	glLoadIdentity();
}

void Sputnik::display() {
	glLoadIdentity();

	Vector3d parentPos = parent->getPosition();

	glTranslated((parentPos.x + (position.x  * Constants::sputniksDistDisplayFactor)) / Constants::metersPerPixel,
		(parentPos.y + (position.y * Constants::sputniksDistDisplayFactor))/ Constants::metersPerPixel,
		(parentPos.z + (position.z * Constants::sputniksDistDisplayFactor))/ Constants::metersPerPixel);

	float materialAmb[] = { color.x * 0.05f, color.y *  0.05f, color.z * 0.05f, 1.0f };
	float materialDiff[] = { 0.4f + color.x*0.2f, 0.4f + color.y*0.2f, 0.4f + color.z*0.2f, 1.0f };

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmb);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiff);

	glutSolidSphere(Constants::sputniksDiamDisplayFactor * diameter / Constants::metersPerPixel, 180, 180);
	glLoadIdentity();
}

SpaceObjectType Star::getType() {
	return STAR;
}

SpaceObjectType Planet::getType() {
	return PLANET;
}

SpaceObjectType Sputnik::getType() {
	return SPUTNIK;
}

const char * SpaceObject::typeStrRepr(SpaceObjectType type) {
	static const char * repr[] = {
		"star",
		"planet",
		"sputnik",
		"teapot"
	};

	return repr[type];
}