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

SpaceObject::SpaceObject(const char * name, double mass, double diameter, Vector3f & color, Vector3d & position, Vector3d & velocity)
{
	this->name = name;
	this->mass = mass;
	this->diameter = diameter;
	this->color = color;
	this->position = position;
	this->velocity = velocity;

	isInteracting = true;
}

void SpaceObject::setDefaultFactors()
{
	SpaceObject::G = default_G;
	SpaceObject::timeAccel = default_timeAccel;
	SpaceObject::metersPerPixel = default_metersPerPixel;
	//Star::diamDisplayFactor = default_starsDiamDisplayFactor;
	//Planet::diamDisplayFactor = default_planetsDiamDisplayFactor;
	//Sputnik::diamDisplayFactor = default_sputniksDiamDisplayFactor;
	//Sputnik::distDisplayFactor = default_sputniksDistDisplayFactor;
}

void SpaceObject::interactWith(SpaceObject * spObj) {
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

const char * SpaceObject::getName() {
	return name.c_str();
}

double SpaceObject::getMass() {
	return mass;
}

double SpaceObject::getDiameter() {
	return diameter;
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
	//sputniks.push_back(sputnik);
}

Star::Star(int glLight, const char * name, double mass, double diameter, Vector3f & color, Vector3d & position, Vector3d & velocity) :
	SpaceObject(name, mass, diameter, color, position, velocity) {
	if (glLight < GL_LIGHT0 || glLight > GL_LIGHT7) {
		glLight = GL_LIGHT0;
	}
	parent = NULL;

	glEnable(glLight);
	this->glLight = glLight;
	float lightAmb[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(glLight, GL_AMBIENT, lightAmb);
	//glLightf(glLight, GL_CONSTANT_ATTENUATION, 1.0);
	glLightf(glLight, GL_LINEAR_ATTENUATION, 6.0e-5);
	glLightf(glLight, GL_QUADRATIC_ATTENUATION, 2.0e-5);
};

void Star::update(std::list<SpaceObject *> spObjs){
	static Vector3d accelsDiff;

	gravityAccel.nullify();

	if (!isInteracting) return;

	for (SpaceObject * spObj : spObjs) {
		if ((spObj == this) || (spObj->getType() == PLANET) || (spObj->getType() == SPUTNIK)) continue;

		interactWith(spObj);
	}

	resultAccel = gravityAccel + accelsDiff;

	if (isInteracting) accelsDiff = resultAccel - gravityAccel;
	else accelsDiff.nullify();
	
	updatePosition();
}

void Planet::update(std::list<SpaceObject *> spObjs){
	static Vector3d accelsDiff;

	gravityAccel.nullify();

	if (!isInteracting) return;

	for (SpaceObject * spObj : spObjs) {
		if ((spObj == this) || (spObj->getType() == SPUTNIK) || (spObj->getType() == PLANET)) continue;

		interactWith(spObj);
	}

	resultAccel = gravityAccel + accelsDiff;
	
	if (isInteracting) accelsDiff = resultAccel - gravityAccel;
	else accelsDiff.nullify();
	
	updatePosition();
}

void Sputnik::update(std::list<SpaceObject *> spObjs){
	static Vector3d accelsDiff;

	gravityAccel.nullify();

	if (!isInteracting) return;

	for (SpaceObject * spObj : spObjs) {
		if ((spObj == this) || (spObj->getType() == STAR) || ((spObj->getType() == PLANET) && (spObj != parent))) continue;

		interactWith(spObj);
	}

	resultAccel = gravityAccel + accelsDiff;
	
	if (isInteracting) accelsDiff = resultAccel - gravityAccel;
	else accelsDiff.nullify();

	updatePosition();
}

void Star::display() {
	glPushMatrix();

	glTranslated(position.x / metersPerPixel, position.y / metersPerPixel, position.z / metersPerPixel);

	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_BLEND);
	glEnable(GL_ALPHA_TEST);
	glColor4f(color.x, color.y, color.z, 0.7f);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	float lightPos[] = { position.x / metersPerPixel, position.y / metersPerPixel, position.z / metersPerPixel, 1.0f };
	float lightDiff[] = { color.x, color.y, color.z, 1.0f };

	glLightfv(glLight, GL_POSITION, lightPos);
	glLightfv(glLight, GL_DIFFUSE, lightDiff);
	
	glutSolidSphere(diamDisplayFactor * diameter / metersPerPixel, 24, 24);

	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_BLEND);
	glDisable(GL_ALPHA_TEST);
	glPopMatrix();
}

void Planet::display() {

	glPushMatrix();

	glTranslated(position.x / metersPerPixel, position.y / metersPerPixel, position.z / metersPerPixel);
	
	float materialAmb[] = { color.x * 0.05f, color.y * 0.05f, color.z * 0.05f, 1.0f };
    float materialDiff[] = { 0.4f + color.x*0.2f, 0.4f + color.y*0.2f, 0.4f + color.z*0.2f, 1.0f };
	
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmb);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiff);
	
	glutSolidSphere(diamDisplayFactor * diameter / metersPerPixel, 24, 24);
	glPopMatrix();
}

void Sputnik::display() {
	glPushMatrix();

	Vector3d parentPos = parent->getPosition();

	glTranslated((parentPos.x + (position.x  * distDisplayFactor)) / metersPerPixel,
		(parentPos.y + (position.y * distDisplayFactor))/ metersPerPixel,
		(parentPos.z + (position.z * distDisplayFactor))/ metersPerPixel);

	float materialAmb[] = { color.x * 0.05f, color.y *  0.05f, color.z * 0.05f, 1.0f };
	float materialDiff[] = { 0.4f + color.x*0.2f, 0.4f + color.y*0.2f, 0.4f + color.z*0.2f, 1.0f };

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmb);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiff);

	glutSolidSphere(diamDisplayFactor * diameter / metersPerPixel, 24, 24);
	glPopMatrix();
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