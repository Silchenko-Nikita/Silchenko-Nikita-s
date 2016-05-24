#include "stdafx.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "SpaceObjects.h"
#include "constants.h"
#include "utils.h"


double SpaceObject::G = 6.67408e-11;
double SpaceObject::timeAccel = 5.0e6;
double SpaceObject::metersPerPixel = 3.0e9;
double Star::diamDisplayFactor = 10.0; // change of diameter of star being displayed
double Planet::diamDisplayFactor = 350.0; // change of diameter of planet being displayed
double Sputnik::diamDisplayFactor = 250.0; // change of diameter of sputnik being displayed
double Sputnik::distDisplayFactor = 20.0; // change of distance from sputnik to it's parent when displayed

int SpaceObject::spObjsNum = 0;
int Star::starsNum = 0;

const bool SpaceObject::interactionArr[]= { true, true, true, true, true };
const bool Star::interactionArr[] = { true, true, true, true, false };
const bool Planet::interactionArr[] = { true, true, false, false, false };
const bool Sputnik::interactionArr[] = { true, true, true, true, false };

SpaceObject::SpaceObject(const char * name, double mass, double diameter, Vector3f & color, Vector3d & position, Vector3d & velocity)
{
	this->name = name;
	this->mass = mass;
	this->diameter = diameter;
	this->color = color;
	this->position = position;
	this->velocity = velocity;

	_isInteracting = true;
	_isActive = true;
	_isVisible = true;
	_isBeingRendered = true;
	_isRemoved = false;

	id = spObjsNum;
	spObjsNum++;
}

Star::Star(const char * name, double mass, double diameter, Vector3f & color, Vector3d & position, Vector3d & velocity) :
	SpaceObject(name, mass, diameter, color, position, velocity) {
	parent = NULL;
	if (starsNum > 7) {
		id = 0;
	}
	else {
		id = starsNum;
	}
	glEnable(GL_LIGHT0 + id);
	starsNum++;
	float lightAmb[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT0 + id, GL_AMBIENT, lightAmb);
	//glLightf(GL_LIGHT0 + id, GL_CONSTANT_ATTENUATION, 1.0);
	glLightf(GL_LIGHT0 + id, GL_LINEAR_ATTENUATION, 4.0e-5);
	glLightf(GL_LIGHT0 + id, GL_QUADRATIC_ATTENUATION, 1.0e-5);
};

Planet::Planet(const char * name, double mass, double diameter, Vector3f & color, Vector3d & position, Vector3d & velocity) :
	SpaceObject(name, mass, diameter, color, position, velocity) {
	parent = NULL;
}

Sputnik::Sputnik(const char * name, double mass, double diameter, Vector3f & color, Planet * parent, Vector3d & relativePos, Vector3d & relativeVel) :
	SpaceObject(name, mass, diameter, color, relativePos, relativeVel) {
	this->parent = parent;
	parent->addSputnik(this);
}

void SpaceObject::interactWith(SpaceObject * spObj) {
	if (!spObj->isInteracting() || !spObj->isBeingRendered()) return;

	Vector3d absPosition = position;
	if (parent != NULL) absPosition += parent->getPosition();

	Vector3d objPosition = spObj->getPosition();
	double objDiameter = spObj->getDiameter(), objMass = spObj->getMass();
	double dist = sqrt(pow(objPosition.x - absPosition.x, 2) + pow(objPosition.y - absPosition.y, 2) + pow(objPosition.z - absPosition.z, 2));

	if (dist > diameter/2 + objDiameter/2) {

		gravityAccel.x += G * (objMass * (objPosition.x - absPosition.x) / pow(dist, 3));
		gravityAccel.y += G * (objMass * (objPosition.y - absPosition.y) / pow(dist, 3));
		gravityAccel.z += G * (objMass * (objPosition.z - absPosition.z) / pow(dist, 3));
	}
}

void SpaceObject::updatePosition() {
	velocity += resultAccel * (Constants::deltaTime * 1.0e-3 * timeAccel);
	position += velocity * (Constants::deltaTime * 1.0e-3  * timeAccel);

	/*if (parent != NULL) {
		Vector3d parPosition = parent->getPosition();
		double dist = sqrt(pow(position.x - parPosition.x, 2) + pow(position.y - parPosition.y, 2) + pow(position.z - parPosition.z, 2));
		if (dist > 500.0 * parent->getDiameter()) {
			position += parPosition;
			boundToParent = false;
		}
	}*/
}

void Planet::addSputnik(Sputnik * sputnik) {
	sputniks.push_back(sputnik);
}

void SpaceObject::update(std::list<SpaceObject *> & spObjs){	
	if (!_isActive) {
		resultAccel.nullify();
		return;
	}

	gravityAccel.nullify();

	if (_isInteracting) {

		for (SpaceObject * spObj : spObjs) {
			if ((spObj == this) || (!interactsWith(spObj->getType()) || (parent != NULL && spObj != parent))) continue;

			interactWith(spObj);
		}
	}
		
	resultAccel = gravityAccel + additionalAccel;
	updatePosition();
}

void Star::display() {
	if (!_isVisible) {
		float lightDiff[] = { 0.0, 0.0, 0.0, 1.0f };
		glLightfv(GL_LIGHT0 + id, GL_DIFFUSE, lightDiff);
		return;
	}

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glTranslated(position.x / metersPerPixel, position.y / metersPerPixel, position.z / metersPerPixel);


	float lightPos[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float lightDiff[] = { color.x, color.y, color.z, 1.0f };

	glLightfv(GL_LIGHT0 + id, GL_POSITION, lightPos);
	glLightfv(GL_LIGHT0 + id, GL_DIFFUSE, lightDiff);

	glEnable(GL_COLOR_MATERIAL);

	glColor3f(color.x, color.y, color.z);
	

	double scale = diamDisplayFactor * diameter / metersPerPixel;
	glScaled(scale, scale, scale);
	glCallList(SPHERE_LIST_ID);

	glDisable(GL_COLOR_MATERIAL);

	glPopMatrix();
}

void Planet::display() {
	if (!_isVisible) return;

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glTranslated(position.x / metersPerPixel, position.y / metersPerPixel, position.z / metersPerPixel);
	
	float materialAmb[] = { color.x * 0.15f, color.y * 0.15f, color.z * 0.15f, 1.0f };
    float materialDiff[] = { 0.6f + color.x*0.2f, 0.6f + color.y*0.2f, 0.6f + color.z*0.2f, 1.0f };
	
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmb);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiff);

	double scale = diamDisplayFactor * diameter / metersPerPixel;
	glScaled(scale, scale, scale);
	glCallList(SPHERE_LIST_ID);

	glPopMatrix();
}

void Sputnik::display() {
	if (!_isVisible) return;

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	Vector3d parentPos = parent->getPosition();

	glTranslated((parentPos.x + (position.x  * distDisplayFactor)) / metersPerPixel,
		(parentPos.y + (position.y * distDisplayFactor))/ metersPerPixel,
		(parentPos.z + (position.z * distDisplayFactor))/ metersPerPixel);

	float materialAmb[] = { color.x * 0.15f, color.y *  0.15f, color.z * 0.15f, 1.0f };
	float materialDiff[] = { 0.6f + color.x*0.2f, 0.6f + color.y*0.2f, 0.6f + color.z*0.2f, 1.0f };

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmb);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiff);

	double scale = diamDisplayFactor * diameter / metersPerPixel;
	glScaled(scale, scale, scale);
	glCallList(SPHERE_LIST_ID);

	glPopMatrix();
}

const char * SpaceObject::typeStrRepr(SpaceObjectType type) {
	static const char * repr[] = {
		"space object",
		"star",
		"planet",
		"sputnik",
		"teapot"
	};

	return repr[type];
}