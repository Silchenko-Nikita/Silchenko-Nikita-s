#include "stdafx.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "SpaceObjects.h"
#include "constants.h"
#include "utils.h"

const double SpaceObject::default_G = 6.67408e-11;
const double SpaceObject::default_timeAccel = 5.0e6;
const double SpaceObject::default_metersPerPixel = 1.0e9;
const double Star::default_diamDisplayFactor = 15.0;
const double Planet::default_diamDisplayFactor = 350.0;
const double Sputnik::default_diamDisplayFactor = 250.0;
const double Sputnik::default_distDisplayFactor = 20.0;

double SpaceObject::G = default_G;
double SpaceObject::timeAccel = default_timeAccel;
double SpaceObject::metersPerPixel = default_metersPerPixel;
double Star::diamDisplayFactor = default_diamDisplayFactor; // change of diameter of star being displayed
double Planet::diamDisplayFactor = default_diamDisplayFactor; // change of diameter of planet being displayed
double Sputnik::diamDisplayFactor = default_diamDisplayFactor; // change of diameter of sputnik being displayed
double Sputnik::distDisplayFactor = default_distDisplayFactor; // change of distance from sputnik to it's parent when displayed

int SpaceObject::spObjsNum = 0;
int Star::starsNum = 0;

const bool SpaceObject::interactionArr[]= { true, true, true, true };
const bool Star::interactionArr[] = {true, false, false, false};
const bool Planet::interactionArr[] = { true, false, false, false };
const bool Sputnik::interactionArr[] = { false, false, false, false };

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
	//glLightf(glLight, GL_CONSTANT_ATTENUATION, 1.0);
	glLightf(GL_LIGHT0 + id, GL_LINEAR_ATTENUATION, 6.0e-5);
	glLightf(GL_LIGHT0 + id, GL_QUADRATIC_ATTENUATION, 2.0e-5);
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

	if (dist > (diameter/2 + objDiameter/2)) {

		gravityAccel.x += G * (objMass * (objPosition.x - absPosition.x) / pow(dist, 3));
		gravityAccel.y += G * (objMass * (objPosition.y - absPosition.y) / pow(dist, 3));
		gravityAccel.z += G * (objMass * (objPosition.z - absPosition.z) / pow(dist, 3));
	}
}

void SpaceObject::updatePosition() {
	velocity += resultAccel * (Constants::deltaTime * 1.0e-3 * timeAccel);
	position += velocity * (Constants::deltaTime * 1.0e-3  * timeAccel);
}

void Planet::addSputnik(Sputnik * sputnik) {
	//sputniks.push_back(sputnik);
}

void SpaceObject::update(std::list<SpaceObject *> & spObjs){
	if (!_isActive) {
		resultAccel.nullify();
		return;
	}

	static Vector3d accelsDiff;

	gravityAccel.nullify();

	if (_isInteracting) {

		for (SpaceObject * spObj : spObjs) {
			if ((spObj == this) || (!interactsWith(spObj->getType()) && (parent != NULL && spObj != parent))) continue;

			interactWith(spObj);
		}
	}

	resultAccel = gravityAccel + accelsDiff;

	if (_isInteracting) accelsDiff = resultAccel - gravityAccel;
	else accelsDiff.nullify();
	
	updatePosition();
}

void Star::display() {
	if (!_isVisible) {
		float lightDiff[] = { 0.0, 0.0, 0.0, 1.0f };
		glLightfv(GL_LIGHT0 + id, GL_DIFFUSE, lightDiff);
		return;
	}

	glPushMatrix();

	glTranslated(position.x / metersPerPixel, position.y / metersPerPixel, position.z / metersPerPixel);


	float lightPos[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float lightDiff[] = { color.x, color.y, color.z, 1.0f };

	glLightfv(GL_LIGHT0 + id, GL_POSITION, lightPos);
	glLightfv(GL_LIGHT0 + id, GL_DIFFUSE, lightDiff);
	
	glEnable(GL_COLOR_MATERIAL);

	glColor3f(color.x, color.y, color.z);
	glutSolidSphere(diamDisplayFactor * diameter / metersPerPixel, 24, 24);

	glDisable(GL_COLOR_MATERIAL);

	glPopMatrix();
}

void Planet::display() {
	if (!_isVisible) return;

	glPushMatrix();

	glTranslated(position.x / metersPerPixel, position.y / metersPerPixel, position.z / metersPerPixel);
	
	float materialAmb[] = { color.x * 0.1f, color.y * 0.1f, color.z * 0.1f, 1.0f };
    float materialDiff[] = { 0.4f + color.x*0.2f, 0.4f + color.y*0.2f, 0.4f + color.z*0.2f, 1.0f };
	
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmb);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiff);

	
	glutSolidSphere(diamDisplayFactor * diameter / metersPerPixel, 24, 24);
	glPopMatrix();
}

void Sputnik::display() {
	if (!_isVisible) return;

	glPushMatrix();

	Vector3d parentPos = parent->getPosition();

	glTranslated((parentPos.x + (position.x  * distDisplayFactor)) / metersPerPixel,
		(parentPos.y + (position.y * distDisplayFactor))/ metersPerPixel,
		(parentPos.z + (position.z * distDisplayFactor))/ metersPerPixel);

	float materialAmb[] = { color.x * 0.1f, color.y *  0.1f, color.z * 0.1f, 1.0f };
	float materialDiff[] = { 0.4f + color.x*0.2f, 0.4f + color.y*0.2f, 0.4f + color.z*0.2f, 1.0f };

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmb);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiff);

	glutSolidSphere(diamDisplayFactor * diameter / metersPerPixel, 24, 24);
	glPopMatrix();
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