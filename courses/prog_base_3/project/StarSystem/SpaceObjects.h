#pragma once

#include <list>

#include <AntTweakBar.h>  
#include <string>  

#include "vector3d.h"
#include "TwManager.h"

enum SpaceObjectType{
	STAR, PLANET, SPUTNIK, TEAPOT
};

class SpaceObject
{

protected:
	std::string name;

	SpaceObject * parent; // defines the meaning of accel, velocity and position (if they are relative to parent or absolute)

	double mass;
	double diameter;
	
	Vector3d color;
	
	Vector3d accel; 
	Vector3d velocity; 
	Vector3d position;

	void interactWith(SpaceObject * spObj);
	void updatePosition();
public:
	SpaceObject(const char * name, double mass, double diameter, Vector3d & color, Vector3d & position, Vector3d & velocity);
	virtual ~SpaceObject() {};
	
	const char * getName();
	double getMass();
	double getDiameter();
	Vector3d getAccel();
	Vector3d getVelocity();
	Vector3d getPosition();
	const char * getTypeStr();

	friend void addSpObjToTwBar(TwBar * bar, SpaceObject * spObj);

	friend void TwManager::addSpObjToTwBar(TwBar * bar, SpaceObject * spObj);
	friend void TW_CALL TwCB::spObj_setMantissa(const void *value, void *clientData);
	friend void TW_CALL TwCB::spObj_getMantissa(void *value, void *clientData);
	friend void TW_CALL TwCB::spObj_setExp(const void *value, void *clientData);
	friend void TW_CALL TwCB::spObj_getExp(void *value, void *clientData);
	
	virtual void update(std::list<SpaceObject *> spObjs) = 0;
	virtual void display() = 0;
	virtual SpaceObjectType getType() = 0;

	static const char * typeStrRepr(SpaceObjectType type);
};

class Star: public SpaceObject
{
public:
	Star(const char * name, double mass, double diameter, Vector3d & color, Vector3d & position, Vector3d & velocity) :
		SpaceObject(name, mass, diameter, color, position, velocity) { parent = NULL; };
	void update(std::list<SpaceObject *> spObjs);
	void display();
	SpaceObjectType getType();
};

class Sputnik;

class Planet : public SpaceObject
{
	std::list<SpaceObject *> sputniks;
public:
	Planet(const char * name, double mass, double diameter, Vector3d & color, Vector3d & position, Vector3d & velocity) :
		SpaceObject(name, mass, diameter, color, position, velocity) { parent = NULL; };
	void addSputnik(Sputnik * sputnik);
	void update(std::list<SpaceObject *> spObjs);
	void display();
	SpaceObjectType getType();
};

class Sputnik : public SpaceObject
{
public:
	Sputnik(const char * name, double mass, double diameter, Vector3d & color, Planet * parent, Vector3d & relativePos, Vector3d & relativeVel):
		SpaceObject(name, mass, diameter, color, relativePos, relativeVel) { this->parent = parent; parent->addSputnik(this); };
	void update(std::list<SpaceObject *> spObjs);
	void display();
	SpaceObjectType getType();
};