#pragma once

#include <list>

#include <GL/freeglut.h> 
#include <AntTweakBar.h>  
#include <string>  

#include "vector3.h"
#include "ControlPane.h"

enum SpaceObjectType{
	STAR, PLANET, SPUTNIK, TEAPOT
};

class ControlPane;

class SpaceObject
{

protected:
	std::string name;

	SpaceObject * parent; // defines the meaning of accel, velocity and position (if they are relative to parent or absolute)

	double mass;
	double diameter;
	
	Vector3f color;
	
	Vector3d accel; 
	Vector3d velocity; 
	Vector3d position;

	void interactWith(SpaceObject * spObj);
	void updatePosition();
public:
	SpaceObject(const char * name, double mass, double diameter, Vector3f & color, Vector3d & position, Vector3d & velocity);
	virtual ~SpaceObject() {};
	
	const char * getName();
	double getMass();
	double getDiameter();
	Vector3d getAccel();
	Vector3d getVelocity();
	Vector3d getPosition();
	SpaceObject * getParent();
	const char * getTypeStr();

	friend class ControlPane;
	
	virtual void update(std::list<SpaceObject *> spObjs) = 0;
	virtual void display() = 0;
	virtual SpaceObjectType getType() = 0;

	static const char * typeStrRepr(SpaceObjectType type);
};

class Star: public SpaceObject
{
	int glLight; // from GL_LIGHT0 to GL_LIGHT7
public:
	Star(int glLight, const char * name, double mass, double diameter, Vector3f & color, Vector3d & position, Vector3d & velocity) :
		SpaceObject(name, mass, diameter, color, position, velocity) {
		if (glLight < GL_LIGHT0 || glLight > GL_LIGHT7) {
			glLight = GL_LIGHT0;
		}
		float lightAmb[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glLightfv(glLight, GL_AMBIENT, lightAmb);
		glEnable(glLight);
		this->glLight = glLight;
		parent = NULL;
	};
	void update(std::list<SpaceObject *> spObjs);
	void display();
	SpaceObjectType getType();
};

class Sputnik;

class Planet : public SpaceObject
{
	std::list<SpaceObject *> sputniks;
public:
	Planet(const char * name, double mass, double diameter, Vector3f & color, Vector3d & position, Vector3d & velocity) :
		SpaceObject(name, mass, diameter, color, position, velocity) { parent = NULL; };
	void addSputnik(Sputnik * sputnik);
	void update(std::list<SpaceObject *> spObjs);
	void display();
	SpaceObjectType getType();
};

class Sputnik : public SpaceObject
{
public:
	Sputnik(const char * name, double mass, double diameter, Vector3f & color, Planet * parent, Vector3d & relativePos, Vector3d & relativeVel):
		SpaceObject(name, mass, diameter, color, relativePos, relativeVel) { this->parent = parent; parent->addSputnik(this); };
	void update(std::list<SpaceObject *> spObjs);
	void display();
	SpaceObjectType getType();
};