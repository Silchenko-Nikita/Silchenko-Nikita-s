#pragma once

#include <list>
#include <string>

#include "twCB.h"

class SpaceObject;

// encapsulates utilities for inheritors
class Pane {
protected:
	std::list<void *> bufferList; // list of var with default values to avoid memory leak
	TwBar * twBar = NULL;

	void moveOneGroupToAnother(const char * moving, const char * target);

	void addDoubleVar(const char * varName, double * var, bool isNonnegat, bool ableToSetDefault = true);
	void addVector3dVar(const char * varName, Vector3d * var, bool isNonnegat, bool ableToSetDefault = true);

	void removeDoubleVar(const char * varName);
	void removeVector3dVar(const char * varName);

	void addBoolVarToGroup(const char * groupName, const char * varName, bool * var);
	void addDoubleVarToGroup(const char * groupName, const char * varName, double * var, bool isNonnegat, bool ableToSetDefault = true);
	void addColorVarToGroup(const char * groupName, const char * varName, Vector3f * var, bool ableToSetDefault = true);
	void addVector3dVarToGroup(const char * groupName, const char * varName, Vector3d * var, bool isNonnegat, bool ableToSetDefault = true);
	void addStrVarToGroup(const char * groupName, const char * varName, std::string * var, bool ableToSetDefault = true);
	void addButtonToGroup(const char * groupName, const char * buttonName, TwButtonCallback buttonCB, void * clientData);
	
	Pane() {};
	Pane(const Pane &) {};
	Pane& operator=(Pane &) {};
public:
	void addSpaceObject(SpaceObject * spObj, bool ableToSetDefault = true);
	void removeSpaceObject(SpaceObject * spObj);
	~Pane();
};

class ControlPane : Pane {
	static ControlPane * instance;

	ControlPane() {};
	ControlPane(const ControlPane &) {};
	ControlPane& operator=(ControlPane &) {};
public:
	static ControlPane * getInstance();
	void init();
	void addSpaceObject(SpaceObject * spObj);
};

class NewSpaceObjectPane : Pane{
	static NewSpaceObjectPane * instance;
	
	ControlPane * ctrlPane = NULL; // ControlPane is needed to let NewSpaceObjectPane know where to add SpaceObject for modifying
	SpaceObject * newSpObj = NULL;

	NewSpaceObjectPane() {};
	NewSpaceObjectPane(const NewSpaceObjectPane &) {};
	NewSpaceObjectPane& operator=(NewSpaceObjectPane &) {};
public:
	static NewSpaceObjectPane * getInstance(ControlPane * ctrlPane);
	void init(ControlPane * ctrlPane);
	void createSpaceObject();
	~NewSpaceObjectPane();
};