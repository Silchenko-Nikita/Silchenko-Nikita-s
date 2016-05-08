#pragma once

#include <list>

#include <GL/freeglut.h> 
#include <AntTweakBar.h>  
#include <string>  

#include "vector3.h"
#include "panes.h"

struct Vector3f;

enum SpaceObjectType{
	STAR, PLANET, SPUTNIK, TEAPOT
};

class SpaceObject
{
	static int spObjsNum;
protected:
	static const bool interactionArr[TEAPOT + 1]; // if obj interactsWith any of SpaceObjectType
	static const double default_G;
	static const  double default_timeAccel;
	static const double default_metersPerPixel;
	static const double default_diamDisplayFactor;

	static double G;
	static double diamDisplayFactor;
	static double timeAccel;
	static double metersPerPixel ; // abstract pixel, not necessary screen

	std::string name;
	int id;

	SpaceObject * parent; // defines the meaning of accel, velocity and position (if they are relative to parent or absolute)

	bool _isInteracting;
	bool _isActive;
	bool _isVisible;
	bool _isBeingRendered;

	double mass;
	double diameter;
	
	Vector3f color;
	
	Vector3d gravityAccel;
	Vector3d resultAccel;

	Vector3d velocity; 
	Vector3d position;
	
	void interactWith(SpaceObject * spObj);
	void updatePosition();
public:
	SpaceObject(){}
	SpaceObject(const char * name, double mass, double diameter, Vector3f & color, Vector3d & position, Vector3d & velocity);
	virtual ~SpaceObject() {};
	
	const char * getName() { return name.c_str(); };
	int getId() { return id; };
	double getMass() { return mass; };
	double getDiameter() { return diameter; };
	Vector3d getVelocity() { return velocity; };
	Vector3d getPosition() { return position; };
	SpaceObject * getParent() { return parent; };
	const char * getTypeStr() { return typeStrRepr(getType()); };

	bool isBeingRendered() { return _isBeingRendered; };
	bool isVisible() { return _isVisible; };
	bool isActive() { return _isActive; };
	bool isInteracting() { return _isInteracting; };

	void setBeingRendered(bool value) { _isBeingRendered = value; };

	virtual bool interactsWith(SpaceObjectType type) { return interactionArr[type]; };
	virtual void update(std::list<SpaceObject *> & spObjs);
	virtual void display() = 0;
	virtual SpaceObjectType getType() = 0;

	static const char * typeStrRepr(SpaceObjectType type);
	
	friend class Pane;
	friend class ControlPane;
	friend class NewSpaceObjectPane;
};

class Star: public SpaceObject
{
	static const bool interactionArr[TEAPOT + 1]; // if obj interactsWith any of SpaceObjectType

	static const double default_diamDisplayFactor;

	static double diamDisplayFactor;

	static int starsNum; // for GL_LIGHT

	int id; // for GL_LIGHT from 0 to 7
public:
	Star(){}
	Star(const char * name, double mass, double diameter, Vector3f & color, Vector3d & position, Vector3d & velocity);
	bool interactsWith(SpaceObjectType type) { return interactionArr[type]; };
	void display();
	SpaceObjectType getType() { return STAR; };

	friend class Pane;
	friend class ControlPane;
	friend class NewSpaceObjectPane;
};

class Sputnik;

class Planet : public SpaceObject
{
	static const bool interactionArr[TEAPOT + 1]; // if obj interactsWith any of SpaceObjectType

	static const double default_diamDisplayFactor;

	static double diamDisplayFactor;

	//std::list<SpaceObject *> sputniks;
public:
	Planet(){};
	Planet(const char * name, double mass, double diameter, Vector3f & color, Vector3d & position, Vector3d & velocity);
	void addSputnik(Sputnik * sputnik);
	bool interactsWith(int type) { return interactionArr[type]; };
	void display();
	SpaceObjectType getType() { return PLANET; };

	friend class Pane;
	friend class ControlPane;
	friend class NewSpaceObjectPane;
};

class Sputnik : public SpaceObject
{
	static const bool interactionArr[TEAPOT + 1]; // if obj interactsWith any of SpaceObjectType

	static const double default_diamDisplayFactor;
	static const double default_distDisplayFactor;

	static double diamDisplayFactor;
	static double distDisplayFactor;
public:
	Sputnik() {};
	Sputnik(const char * name, double mass, double diameter, Vector3f & color, Planet * parent, Vector3d & relativePos, Vector3d & relativeVel);
	bool interactsWith(SpaceObjectType type) { return interactionArr[type]; };
	void display();
	SpaceObjectType getType() { return SPUTNIK;  };

	friend class Pane;
	friend class ControlPane;
	friend class NewSpaceObjectPane;
};