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
	static const double default_G;
	static const  double default_timeAccel;
	static const double default_metersPerPixel;
	static const double default_diamDisplayFactor;

	static double G;
	static double diamDisplayFactor;
	static double timeAccel;
	static double metersPerPixel ; // abstract pixel, not necessary screen

	std::string name;

	SpaceObject * parent; // defines the meaning of accel, velocity and position (if they are relative to parent or absolute)

	bool isInteracting;

	double mass;
	double diameter;
	
	Vector3f color;
	
	Vector3d gravityAccel;
	Vector3d resultAccel;

	Vector3d velocity; 
	Vector3d position;

	static void setDefaultFactors();
	
	void interactWith(SpaceObject * spObj);
	void updatePosition();
public:
	SpaceObject(const char * name, double mass, double diameter, Vector3f & color, Vector3d & position, Vector3d & velocity);
	virtual ~SpaceObject() {};
	
	const char * getName();
	double getMass();
	double getDiameter();
	Vector3d getVelocity();
	Vector3d getPosition();
	SpaceObject * getParent();
	const char * getTypeStr();

	virtual void update(std::list<SpaceObject *> spObjs) = 0;
	virtual void display() = 0;
	virtual SpaceObjectType getType() = 0;

	static const char * typeStrRepr(SpaceObjectType type);
	
	friend class ControlPane;
};

class Star: public SpaceObject
{
	static const double default_diamDisplayFactor;

	static double diamDisplayFactor;

	int glLight; // from GL_LIGHT0 to GL_LIGHT7
public:
	Star(int glLight, const char * name, double mass, double diameter, Vector3f & color, Vector3d & position, Vector3d & velocity);
	void update(std::list<SpaceObject *> spObjs);
	void display();
	SpaceObjectType getType();

	friend class ControlPane;
};

class Sputnik;

class Planet : public SpaceObject
{
	static const double default_diamDisplayFactor;

	static double diamDisplayFactor;

	//std::list<SpaceObject *> sputniks;
public:
	Planet(const char * name, double mass, double diameter, Vector3f & color, Vector3d & position, Vector3d & velocity) :
		SpaceObject(name, mass, diameter, color, position, velocity) { parent = NULL; };
	void addSputnik(Sputnik * sputnik);
	void update(std::list<SpaceObject *> spObjs);
	void display();
	SpaceObjectType getType();

	friend class ControlPane;
};

class Sputnik : public SpaceObject
{
	static const double default_diamDisplayFactor;
	static const double default_distDisplayFactor;

	static double diamDisplayFactor;
	static double distDisplayFactor;
public:
	Sputnik(const char * name, double mass, double diameter, Vector3f & color, Planet * parent, Vector3d & relativePos, Vector3d & relativeVel):
		SpaceObject(name, mass, diameter, color, relativePos, relativeVel) { this->parent = parent; parent->addSputnik(this); };
	void update(std::list<SpaceObject *> spObjs);
	void display();
	SpaceObjectType getType();

	friend class ControlPane;
};