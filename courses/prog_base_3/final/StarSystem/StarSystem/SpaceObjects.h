#pragma once

#include <list>
#include <vector>

#include <GL/freeglut.h> 
#include <AntTweakBar.h>  
#include <string>  

#include "SOIL.h"

#include "vector3.h"
#include "ControlPane.h"

struct Vector3f;

enum SpaceObjectType{
	GENERAL, STAR, PLANET, SPUTNIK, TEAPOT
};

class SpaceObject
{
	static int spObjsNum;
protected:
	static const bool interactionArr[TEAPOT + 1]; // if obj interactsWith any of SpaceObjectType

	static double G;
	static double diamDisplayFactor;
	static double timeAccel;
	static double metersPerPixel; // abstract pixel, not necessary screen
	
	GLuint texture;

	std::string name;
	int id;

	SpaceObject * parent = NULL; // defines the meaning of objects accel, velocity and position (if they are relative to parent or absolute)

	bool _isInteracting = true;
	bool _isActive = true;
	bool _isVisible = true;
	bool _isBeingRendered = true;
	bool _isTextured = false;
	bool _isRemoved = false; // if interacts with parent only (for providing easy way to control position)

	double mass;
	double diameter;
	double rotationAnglePerYear;
	double resAtmosphereRot;
	double resRot = 0.0;

	Vector3f color;
	
	Vector3d gravityAccel;
	Vector3d additionalAccel;
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

	virtual void loadTexture(const char * filename);

	bool isBeingRendered() { return _isBeingRendered; };
	bool isVisible() { return _isVisible; };
	bool isActive() { return _isActive; };
	bool isTextured() { return _isTextured; };
	bool isInteracting() { return _isInteracting; };
	bool isRemoved() { return _isRemoved; };

	void remove() { _isRemoved = true; }
	virtual void setBeingRendered(bool value) { _isBeingRendered = value; };

	virtual bool interactsWith(SpaceObjectType type) { return interactionArr[type]; };
	virtual void update(std::list<SpaceObject *> & spObjs);
	virtual void display(GLUquadricObj * quadric) {};
	virtual SpaceObjectType getType() { return GENERAL; };
	virtual double getDiamDisplayFactor() { return diamDisplayFactor; }

	static double getMetersPerPixel() { return metersPerPixel; }
	static const char * typeStrRepr(SpaceObjectType type);
	
	friend class Pane;
	friend class ControlPane;
	friend class NewSpaceObjectPane;
};

class Star: public SpaceObject
{
	static const bool interactionArr[TEAPOT + 1]; // if obj interactsWith any of SpaceObjectType

	static double diamDisplayFactor;
	double linearAtten, quadraticAtten;

	static int starsNum; // for GL_LIGHT

	int id; // for GL_LIGHT from 0 to 7
public:
	Star(){}
	Star(const char * name, double mass, double diameter, Vector3f & color, Vector3d & position, Vector3d & velocity);
	bool interactsWith(SpaceObjectType type) { return interactionArr[type]; };
	void display(GLUquadricObj * quadric);
	SpaceObjectType getType() { return STAR; };

	double getDiamDisplayFactor() { return diamDisplayFactor; }

	void setBeingRendered(bool value) {
		_isBeingRendered = value; 
		if (value) {
			glEnable(GL_LIGHT0 + id);
		} else {
			glDisable(GL_LIGHT0 + id);
		}
	};

	friend class Pane;
	friend class ControlPane;
	friend class NewSpaceObjectPane;
};

class Sputnik;
struct Ring {
	bool isTextured = true;
	static int texture;
	double diameter, width;
	Vector3f color;
	float quat[4];

	Ring(double diameter, double width, Vector3f color) {
		this->diameter = diameter;
		this->width = width;
		this->color = color;

		quat[0] = quat[1] = quat[2] = 0.0f;
		quat[3] = 1.0f;
	}

	Ring(){}


	static void loadTexture(const char * filename);
};

class Planet : public SpaceObject
{
	static const bool interactionArr[TEAPOT + 1]; // if obj interactsWith any of SpaceObjectType

	static double diamDisplayFactor;
	Ring ring;
	int atmosphereTexture = 0, ringTexture = 0, textureListId = 0;

	static std::vector<int> planetTextures;
	std::list<Sputnik *> sputniks;
public:
	Planet(){}
	Planet(const char * name, double mass, double diameter, Vector3f & color, Vector3d & position, Vector3d & velocity, Ring  & ring = Ring(0.0, 0.0, Vector3f(0.7f, 0.7f, 0.7f)));
	void addSputnik(Sputnik * sputnik);
	bool interactsWith(SpaceObjectType type) { return interactionArr[type]; };
	std::list<Sputnik *> & getSputniksList() { return sputniks; };
	void display(GLUquadricObj * quadric);
	SpaceObjectType getType() { return PLANET; };
	double getDiamDisplayFactor() { return diamDisplayFactor; }

	void setNextTexture();

	void loadTexture(const char * filename);
	void loadAtmosphereTexture(const char * filename);

	friend class Pane;
	friend class ControlPane;
	friend class NewSpaceObjectPane;
};

class Sputnik : public SpaceObject
{
	static const bool interactionArr[TEAPOT + 1]; // if obj interactsWith any of SpaceObjectType

	static double diamDisplayFactor;
	static double distDisplayFactor;
public:
	Sputnik() {};
	Sputnik(const char * name, double mass, double diameter, Vector3f & color, Planet * parent, Vector3d & relativePos, Vector3d & relativeVel);
	bool interactsWith(SpaceObjectType type) { return interactionArr[type]; };
	void display(GLUquadricObj * quadric);
	SpaceObjectType getType() { return SPUTNIK;  };
	double getDiamDisplayFactor() { return diamDisplayFactor; }
	double getDistDisplayFactor() { return distDisplayFactor; }

	friend class Pane;
	friend class ControlPane;
	friend class NewSpaceObjectPane;
};

class Teapot : public SpaceObject
{
	static const bool interactionArr[TEAPOT + 1]; // if obj interactsWith any of SpaceObjectType

	static double diamDisplayFactor;
public:
	Teapot() {};
	Teapot(const char * name, double mass, double diameter, Vector3f & color, Vector3d & relativePos, Vector3d & relativeVel) : 
	SpaceObject(name, mass, diameter, color, relativePos, relativeVel){};
	bool interactsWith(SpaceObjectType type) { return interactionArr[type]; };
	void display(GLUquadricObj * quadric);
	SpaceObjectType getType() { return TEAPOT; };

	double getDiamDisplayFactor() { return diamDisplayFactor; }

	friend class Pane;
	friend class ControlPane;
	friend class NewSpaceObjectPane;
};