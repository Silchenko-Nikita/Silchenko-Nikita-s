#include "stdafx.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "SpaceObjects.h"
#include "constants.h"
#include "utils.h"

#define EARTH_TEXTURE 1
#define ATMOSPHERE_TEXTURE 2

double SpaceObject::G = 6.67408e-11;
double SpaceObject::timeAccel = 5.0e6;
double SpaceObject::metersPerPixel = 2.0e10;
double SpaceObject::diamDisplayFactor = 0.0;
double Star::diamDisplayFactor = 10.0; // change of diameter of star being displayed
double Planet::diamDisplayFactor = 450.0; // change of diameter of planet being displayed
double Sputnik::diamDisplayFactor = 150.0; // change of diameter of sputnik being displayed
double Sputnik::distDisplayFactor = 15.0; // change of distance from sputnik to it's parent when displayed
double Teapot::diamDisplayFactor = 1.5e10; // change of diameter of teapot being displayed

int Ring::texture;

std::vector<int> Planet::planetTextures;

int SpaceObject::spObjsNum = 0;
int Star::starsNum = 0;

const bool SpaceObject::interactionArr[]= { true, true, true, true, true };
const bool Star::interactionArr[] = { true, true, true, true, false };
const bool Planet::interactionArr[] = { true, true, false, false, false };
const bool Sputnik::interactionArr[] = { true, true, true, true, false };
const bool Teapot::interactionArr[] = { true, true, true, false, false };

SpaceObject::SpaceObject(const char * name, double mass, double diameter, Vector3f & color, Vector3d & position, Vector3d & velocity)
{
	this->name = name;
	this->mass = mass;
	this->diameter = diameter;
	this->color = color;
	this->position = position;
	this->velocity = velocity;
	texture = 0;
	rotationAnglePerYear = 0.0;
	resRot = 0.0;

	id = spObjsNum;
	spObjsNum++;
}

Star::Star(const char * name, double mass, double diameter, Vector3f & color, Vector3d & position, Vector3d & velocity) :
	SpaceObject(name, mass, diameter, color, position, velocity) {
	parent = NULL;
	linearAtten = 1.0;
	quadraticAtten = 1.0;
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
};

Planet::Planet(const char * name, double mass, double diameter, Vector3f & color, Vector3d & position, Vector3d & velocity, Ring  & ring) :
	SpaceObject(name, mass, diameter, color, position, velocity) {
	this->ring = ring;
	atmosphereTexture = 0;
	ringTexture = 0;
	parent = NULL;
	textureListId = 0;
	resAtmosphereRot = 0.0;
}

void SpaceObject::loadTexture(const char * filename) {
	_isTextured = true;
	texture = SOIL_load_OGL_texture(filename, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void Planet::loadTexture(const char * filename) {
	SpaceObject::loadTexture(filename);
	planetTextures.push_back(texture);
	textureListId = planetTextures.size() - 1;
}

void Planet::setNextTexture() {
	textureListId = (textureListId + 1) % planetTextures.size();
	texture = planetTextures.at(textureListId);

	if (texture == EARTH_TEXTURE) atmosphereTexture = ATMOSPHERE_TEXTURE;
	else atmosphereTexture = 0;
}


void Ring::loadTexture(const char * filename) {
	Ring::texture = SOIL_load_OGL_texture(filename, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void Planet::loadAtmosphereTexture(const char * filename) {
	atmosphereTexture = SOIL_load_OGL_texture(filename, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
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
	else {
		gravityAccel.nullify();
		velocity.nullify();
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

void Star::display(GLUquadricObj * quadric) {
	if (!_isVisible) {
		float lightDiff[] = { 0.0, 0.0, 0.0, 1.0f };
		glLightfv(GL_LIGHT0 + id, GL_DIFFUSE, lightDiff);
		return;
	}

	//glLightf(GL_LIGHT0 + id, GL_CONSTANT_ATTENUATION, 1.0);
	glLightf(GL_LIGHT0 + id, GL_LINEAR_ATTENUATION, 2.0e-12 * linearAtten * SpaceObject::metersPerPixel);
	glLightf(GL_LIGHT0 + id, GL_QUADRATIC_ATTENUATION, 3.0e-26 * quadraticAtten  * SpaceObject::metersPerPixel * SpaceObject::metersPerPixel);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glTranslated(position.x / metersPerPixel, position.y / metersPerPixel, position.z / metersPerPixel);


	float lightPos[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float lightDiff[] = { color.x, color.y, color.z, 1.0f };

	glLightfv(GL_LIGHT0 + id, GL_POSITION, lightPos);
	glLightfv(GL_LIGHT0 + id, GL_DIFFUSE, lightDiff);

	glEnable(GL_COLOR_MATERIAL);

	glColor3f(color.x, color.y, color.z);

	double scale = diamDisplayFactor * diameter/ 2 / metersPerPixel;
	//glScaled(scale, scale, scale);
	//glCallList(SPHERE_LIST_ID);

	glDisable(GL_CULL_FACE);
	if (_isTextured){
		glBindTexture(GL_TEXTURE_2D, texture);

		resRot += rotationAnglePerYear * (Constants::deltaTime * 1.0e-3 * timeAccel) / (3600.0 * 24.0 * 365.0);
		if (resRot > 360.0f) {
			resRot = resRot - 360.0f;
		}

		glRotatef(resRot, 0.0f, 0.0f, 1.0f);
		gluSphere(quadric, scale, 45, 45);
	}else {
		gluQuadricTexture(quadric, GL_FALSE);
		gluSphere(quadric, scale, 45, 45);
		gluQuadricTexture(quadric, GL_TRUE);
	}
	
	glEnable(GL_CULL_FACE);
	glDisable(GL_COLOR_MATERIAL);

	glPopMatrix();
}

void Planet::display(GLUquadricObj * quadric) {
	if (!_isVisible) return;

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glTranslated(position.x / metersPerPixel, position.y / metersPerPixel, position.z / metersPerPixel);
	
	float materialAmb[] = { color.x * 0.15f, color.y * 0.15f, color.z * 0.15f, 1.0f };
	float materialDiff[] = { 0.6f + color.x*0.2f, 0.6f + color.y*0.2f, 0.6f + color.z*0.2f, 1.0f };

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmb);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiff);

	double scale = diamDisplayFactor * diameter/ 2 / metersPerPixel;
	//glScaled(scale, scale, scale);
	//glCallList(SPHERE_LIST_ID);
	
	float rotPerSec;
	if (_isTextured) {
		glBindTexture(GL_TEXTURE_2D, texture);
		rotPerSec = rotationAnglePerYear * (Constants::deltaTime * 1.0e-3 * timeAccel) / (3600.0 * 24.0);
		resRot += rotPerSec;
		if(resRot > 360.0f) {
			resRot = resRot - 360.0f;
		}

		glRotatef(resRot, 0.0f, 0.0f, 1.0f);
		gluSphere(quadric, scale, 45, 45);
	}
	else {
		gluQuadricTexture(quadric, GL_FALSE);
		gluSphere(quadric, scale, 45, 45);
		gluQuadricTexture(quadric, GL_TRUE);
	}

	if (_isTextured && atmosphereTexture > 0) {
		resAtmosphereRot += rotPerSec + 0.1;
		if (resAtmosphereRot > 360.0f) {
			resAtmosphereRot = resAtmosphereRot - 360.0f;
		}

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		glBindTexture(GL_TEXTURE_2D, atmosphereTexture);
		glRotatef(resAtmosphereRot, 0.0f, 0.0f, 1.0f);
		gluSphere(quadric, scale + Planet::diamDisplayFactor * 1.5e5 / SpaceObject::metersPerPixel, 45, 45);
		glDisable(GL_BLEND);
	}
	
	// ring
	if (ring.diameter > diameter && ring.width > 1.0e4){
		float ringMaterialAmb[] = { ring.color.x * 0.15f, ring.color.y * 0.15f, ring.color.z * 0.15f, 1.0f };
		float ringMaterialDiff[] = { 0.6f + ring.color.x*0.2f, 0.6f + ring.color.y*0.2f, 0.6f + ring.color.z*0.2f, 1.0f };

		glMaterialfv(GL_FRONT, GL_AMBIENT, ringMaterialAmb);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, ringMaterialAmb);

		/*float mat[16], mat1[16];
		glGetFloatv(GL_MODELVIEW_MATRIX, mat1);
		Utils::getModifMat(ring.quat, mat1, mat);
		glMultMatrixf(mat);*/

		if (ring.isTextured) {
			glEnable(GL_BLEND);
			glDisable(GL_CULL_FACE);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			glBindTexture(GL_TEXTURE_2D, ring.texture);
			gluDisk(quadric, (ring.diameter / 2 - ring.width) / metersPerPixel * diamDisplayFactor, (ring.diameter / 2) / metersPerPixel * diamDisplayFactor, 45, 45);
			//glutSolidTorus((ring.width /2) / metersPerPixel * diamDisplayFactor, (ring.diameter / 2) / metersPerPixel * diamDisplayFactor, 45, 45);
			glEnable(GL_CULL_FACE);
			glDisable(GL_BLEND);
		}
		else {
			gluQuadricTexture(quadric, GL_FALSE);
			gluDisk(quadric, (ring.diameter / 2 - ring.width) / metersPerPixel * diamDisplayFactor, (ring.diameter / 2) / metersPerPixel * diamDisplayFactor, 45, 45);
			gluQuadricTexture(quadric, GL_TRUE);
		}
	}

	glPopMatrix();
}

void Sputnik::display(GLUquadricObj * quadric) {
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

	double scale = diamDisplayFactor * diameter / 2 / metersPerPixel;
	//glScaled(scale, scale, scale);
	//glCallList(SPHERE_LIST_ID);
	
	if (_isTextured) {
		glBindTexture(GL_TEXTURE_2D, texture);

		resRot += rotationAnglePerYear * (Constants::deltaTime * 1.0e-3 * timeAccel) / (3600.0 * 24.0 * 365.0);
		if (resRot > 360.0f) {
			resRot = resRot - 360.0f;
		}

		glRotatef(resRot, 0.0f, 0.0f, 1.0f);
		gluSphere(quadric, scale, 45, 45);
	}
	else {
		gluQuadricTexture(quadric, GL_FALSE);
		gluSphere(quadric, scale, 45, 45);
		gluQuadricTexture(quadric, GL_TRUE);
	}

	glPopMatrix();
}

void Teapot::display(GLUquadricObj * quadric)
{
	if (!_isVisible) return;

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glTranslated(position.x / metersPerPixel, position.y / metersPerPixel, position.z / metersPerPixel);

	float materialAmb[] = { color.x * 0.15f, color.y * 0.15f, color.z * 0.15f, 1.0f };
	float materialDiff[] = { 0.6f + color.x*0.2f, 0.6f + color.y*0.2f, 0.6f + color.z*0.2f, 1.0f };

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmb);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiff);

	double scale = diamDisplayFactor * diameter / metersPerPixel;
	//glScaled(scale, scale, scale);
	//glCallList(TEAPOT_LIST_ID);
	if (_isTextured) {
		glBindTexture(GL_TEXTURE_2D, texture);
	}
	else {
		glDisable(GL_TEXTURE_2D);
	}

	glDisable(GL_CULL_FACE);
	glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	glutSolidTeapot(scale);
	glEnable(GL_CULL_FACE);

	if (!_isTextured) {
		glEnable(GL_TEXTURE_2D);
	}

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
