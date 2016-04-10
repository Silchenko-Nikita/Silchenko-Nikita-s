#pragma once

#include <vector>

#include <AntTweakBar.h>  

#include "vector3d.h"

enum SpaceObjectTypes{
	STAR, PLANET, SPUTNIK, TEAPOT
};

class SpaceObject;

class SpaceObject
{

protected:
	SpaceObjectTypes type; // for convenience in getting obj's type

	double mass;
	double diameter;
	
	Vector3d color;
	
	Vector3d accel;
	Vector3d speed;
	Vector3d position;

public:
	SpaceObject(double mass, double diameter, Vector3d & color, Vector3d & position, Vector3d & speed);
	virtual ~SpaceObject() {};
	
	void associateWithTwBar(TwBar * bar);
	void interactWith(SpaceObject * spObj);
	
	virtual void update(std::vector<SpaceObject *> spObjs) = 0;
	virtual void display() = 0;
	virtual SpaceObjectTypes getType() = 0;
};

class Star: public SpaceObject
{
public:
	Star(double mass, double diameter, Vector3d & color, Vector3d & position, Vector3d & speed) : SpaceObject(mass, diameter, color, position, speed) {};
	void update(std::vector<SpaceObject *> spObjs);
	void display();
	SpaceObjectTypes getType();
};

class Planet : public SpaceObject
{
public:
	Planet(double mass, double diameter, Vector3d & color, Vector3d & position, Vector3d & speed) : SpaceObject(mass, diameter, color, position, speed) {};
	void update(std::vector<SpaceObject *> spObjs);
	void display();
	SpaceObjectTypes getType();
};

class Sputnik : public SpaceObject
{
public:
	Sputnik(double mass, double diameter, Vector3d & color, Vector3d & position, Vector3d & speed) : SpaceObject(mass, diameter, color, position, speed) {};
	void update(std::vector<SpaceObject *> spObjs);
	void display();
	SpaceObjectTypes getType();
};
