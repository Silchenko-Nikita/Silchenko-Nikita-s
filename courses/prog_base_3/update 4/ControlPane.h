#pragma once

#include <list>
#include <string>

#include "twCB.h"
#include "RenderManager.h"
#include "camera.h"

enum SpaceObjectType;
class SpaceObject;
class Camera;
class RenderManager;

// encapsulates utilities for inheritors
class Pane {
protected:
	std::list<void *> bufferList; // list to avoid memory leak
	std::list<void *> spObjsWithDefault;
	TwBar * twBar = NULL;
	bool _isVisible;

	void moveOneGroupToAnother(const char * moving, const char * target);

	void addDoubleVar(const char * varName, double * var, bool isNonnegat, bool ableToSetDefault = true, bool readOnly = false);
	void addVector3dVar(const char * varName, Vector3d * var, bool isNonnegat, bool ableToSetDefault = true, bool readOnly = false);

	void removeDoubleVar(const char * varName);
	void removeVector3dVar(const char * varName);

	void addBoolVarToGroup(const char * groupName, const char * varName, bool * var);
	void addDoubleVarToGroup(const char * groupName, const char * varName, double * var, bool isNonnegat, bool ableToSetDefault = true, bool readOnly = false);
	void addColorVarToGroup(const char * groupName, const char * varName, Vector3f * var, bool ableToSetDefault = true);
	void addVector3dVarToGroup(const char * groupName, const char * varName, Vector3d * var, bool isNonnegat, bool ableToSetDefault = true, bool readOnly = false);
	void addStrVarToGroup(const char * groupName, const char * varName, std::string * var, bool ableToSetDefault = true);
	void addButtonToGroup(const char * groupName, const char * buttonName, TwButtonCallback buttonCB, void * clientData);

	Pane() {};
	Pane(const Pane &) {};
	Pane& operator=(Pane &) {};
public:
	void addSpaceObject(SpaceObject * spObj, bool ableToSetDefault = true, bool ableToDelete = true, bool closed = true);
	virtual void removeSpaceObject(SpaceObject * spObj);

	void show();
	void hide();
	bool isVisible() { return _isVisible; };
	~Pane();
};

class NewSpaceObjectPane;

class ControlPane : public Pane {
	static ControlPane * instance;
	
	Camera * camera;
	RenderManager * renderManager;
	NewSpaceObjectPane * newSpObjPane;

	ControlPane(Camera * camera, RenderManager * renderManager);
	ControlPane(const ControlPane &) {};
	ControlPane& operator=(ControlPane &) {};
public:
	static ControlPane * getInstance(Camera * camera, RenderManager * renderManager);
	void addSpaceObject(SpaceObject * spObj, bool ableToSetDefault = true, bool ableToDelete = true, bool closed = true);
	void removeSpaceObject(SpaceObject * spObj);
	void showNewSpaceObjectPane();
	void hideNewSpaceObjectPane();
	~ControlPane();
};

class NewSpaceObjectPane : public Pane {
	static NewSpaceObjectPane * instance;

	RenderManager * renderManager = NULL;
	ControlPane * ctrlPane = NULL; // ControlPane is needed to let NewSpaceObjectPane know where to add SpaceObject for modifying
	SpaceObject * newSpObj = NULL;
	
	NewSpaceObjectPane(ControlPane * ctrlPane, RenderManager * renderManager);
	NewSpaceObjectPane(const NewSpaceObjectPane &) {};
	NewSpaceObjectPane& operator=(NewSpaceObjectPane &) {};

public:
	static NewSpaceObjectPane * getInstance(ControlPane * ctrlPane, RenderManager * renderManager);
	void addSpaceObject(SpaceObject * spObj, bool ableToSetDefault = true, bool ableToDelete = true, bool closed = true);
	void createSpaceObject();
	void addNewSpaceObject(SpaceObjectType spObjType, SpaceObject * parent = NULL);
	
	void hide();
	~NewSpaceObjectPane();
};