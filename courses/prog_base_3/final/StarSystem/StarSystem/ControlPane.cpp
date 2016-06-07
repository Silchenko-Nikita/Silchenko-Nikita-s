
#include "stdafx.h"

#include <stdio.h>

#include "ControlPane.h"

#include "RenderManager.h"
#include "camera.h"

ControlPane * ControlPane::instance = NULL;
NewSpaceObjectPane * NewSpaceObjectPane::instance = NULL;

void Pane::moveOneGroupToAnother(const char * moving, const char * target)
{
	char strBuff[256];
	sprintf(strBuff, "'%s'/'%s' group= '%s'", TwGetBarName(twBar), moving, target);
	TwDefine(strBuff);
}

void Pane::addDoubleVar(const char * varName, double * var, bool isNonnegat, int minExp, int maxExp, bool ableToSetDefault, bool readOnly)
{
	char strBuff[256], strBuff1[256];

	sprintf(strBuff, "%s_mantissa", varName);
	sprintf(strBuff1, "label='mantissa' group='%s' min=%s max=9.9 step=0.02 readonly='%s'", varName, isNonnegat ? "0.0" : "-9.9", readOnly ? "true" : "false");
	TwAddVarCB(twBar, strBuff, TW_TYPE_DOUBLE, TwCB::setMantissa, TwCB::getMantissa, var, strBuff1);

	sprintf(strBuff, "%s_exponent", varName);
	sprintf(strBuff1, "label='exponent' group='%s' readonly='%s' min=%d max=%d", varName, readOnly ? "true" : "false", minExp, maxExp);
	TwAddVarCB(twBar, strBuff, TW_TYPE_INT32, TwCB::setExp, TwCB::getExp, var, strBuff1);

	if (ableToSetDefault) {

		VarWithDefault<double> * v = new VarWithDefault<double>(var, *var);

		bufferList.push_back(v);

		addButtonToGroup(varName, "setDefault", TwCB::setDefaultDouble_onButtonClick, v);
	}
}

void Pane::removeDoubleVar(const char * varName) {
	char strBuff[256];

	sprintf(strBuff, "%s_mantissa", varName);
	TwRemoveVar(twBar, strBuff);

	sprintf(strBuff, "%s_exponent", varName);
	TwRemoveVar(twBar, strBuff);

	sprintf(strBuff, "%s_setDefault", varName);
	TwRemoveVar(twBar, strBuff);
}

void Pane::addVector3dVar(const char * varName, Vector3d * var, bool isNonnegat, int minExp, int maxExp, bool ableToSetDefault, bool readOnly) {
	char strBuff[256], strBuff1[256];

	addDoubleVarToGroup(varName, "x", &var->x, isNonnegat, minExp, maxExp, ableToSetDefault, readOnly);
	addDoubleVarToGroup(varName, "y", &var->y, isNonnegat, minExp, maxExp, ableToSetDefault, readOnly);
	addDoubleVarToGroup(varName, "z", &var->z, isNonnegat, minExp, maxExp, ableToSetDefault, readOnly);

	sprintf(strBuff, "'%s'/%s opened=false", TwGetBarName(twBar), varName);
	TwDefine(strBuff);

	if (ableToSetDefault) {
		VarWithDefault<Vector3d> * v = new VarWithDefault<Vector3d>(var, *var);

		addButtonToGroup(varName, "setDefault", TwCB::setDefaultVector3d_onButtonClick, v);
	}
}

void  Pane::removeVector3dVar(const char * varName) {
	char strBuff[256];

	sprintf(strBuff, "%s_x", varName);
	removeDoubleVar(strBuff);

	sprintf(strBuff, "%s_y", varName);
	removeDoubleVar(strBuff);

	sprintf(strBuff, "%s_z", varName);
	removeDoubleVar(strBuff);

	sprintf(strBuff, "%s_setDefault", varName);
	TwRemoveVar(twBar, strBuff);
}

void Pane::addBoolVarToGroup(const char * groupName, const char * varName, bool * var) {
	char strBuff[256], strBuff1[256];

	sprintf(strBuff, "%s_%s", groupName, varName);
	TwAddVarRW(twBar, strBuff, TW_TYPE_BOOLCPP, var, "");
	sprintf(strBuff1, "'%s'/%s label='%s' group='%s'", TwGetBarName(twBar), strBuff, varName, groupName);
	TwDefine(strBuff1);
}

void Pane::addDoubleVarToGroup(const char * groupName, const char * varName, double * var, bool isNonnegat, int minExp, int maxExp, bool ableToSetDefault, bool readOnly)
{
	char strBuff[256], strBuff1[256];

	sprintf(strBuff, "%s_%s", groupName, varName);
	addDoubleVar(strBuff, var, isNonnegat, minExp, maxExp, ableToSetDefault, readOnly);
	sprintf(strBuff1, "'%s'/%s label='%s' opened=false", TwGetBarName(twBar), strBuff, varName);
	TwDefine(strBuff1);
	moveOneGroupToAnother(strBuff, groupName);
}

void Pane::addColorVarToGroup(const char * groupName, const char * varName, Vector3f * var, bool ableToSetDefault)
{
	char strBuff[256], strBuff1[256];

	sprintf(strBuff, "%s_%s", groupName, varName);
	TwAddVarRW(twBar, strBuff, TW_TYPE_COLOR3F, var, "");

	sprintf(strBuff1, "'%s'/%s opened=false label='%s' colormode=rgb", TwGetBarName(twBar), strBuff, varName);
	TwDefine(strBuff1);
	moveOneGroupToAnother(strBuff, groupName);

	if (ableToSetDefault) {
		VarWithDefault<Vector3f> * v = new VarWithDefault<Vector3f>(var, *var);

		addButtonToGroup(strBuff, "setDefault", TwCB::setDefaultVector3f_onButtonClick, v);
	}
}

void Pane::addVector3dVarToGroup(const char * groupName, const char * varName, Vector3d * var, bool isNonnegat, int minExp, int maxExp, bool ableToSetDefault, bool readOnly)
{
	char strBuff[256], strBuff1[256];

	sprintf(strBuff, "%s_%s", groupName, varName);
	addVector3dVar(strBuff, var, isNonnegat, minExp, maxExp, ableToSetDefault, readOnly);

	sprintf(strBuff1, "'%s'/%s opened=false label='%s'", TwGetBarName(twBar), strBuff, varName);
	TwDefine(strBuff1);
	moveOneGroupToAnother(strBuff, groupName);
}

void Pane::addStrVarToGroup(const char * groupName, const char * varName, std::string * var, bool ableToSetDefault)
{
	char strBuff[256], strBuff1[256];

	sprintf(strBuff, "%s_%s", groupName, varName);
	TwAddVarRW(twBar, strBuff, TW_TYPE_STDSTRING, var, "");

	sprintf(strBuff1, "'%s'/%s label='%s'", TwGetBarName(twBar), strBuff, varName);
	TwDefine(strBuff1);
	moveOneGroupToAnother(strBuff, groupName);
}

void Pane::addButtonToGroup(const char * groupName, const char * buttonName, TwButtonCallback buttonCB, void * clientData)
{
	char strBuff[256], strBuff1[256];

	sprintf(strBuff, "%s_%s", groupName, buttonName);
	TwAddButton(twBar, strBuff, buttonCB, clientData, "");
	sprintf(strBuff1, "'%s'/%s label='%s'", TwGetBarName(twBar), strBuff, buttonName);
	TwDefine(strBuff1);
	moveOneGroupToAnother(strBuff, groupName);
}

void Pane::addSpaceObject(SpaceObject * spObj, bool ableToSetTextured, bool ableToSetDefault, bool ableToDelete, bool closed)
{
	char strBuff[256], strBuff1[256], strBuff2[256];

	sprintf(strBuff, "spaceObject#%d", spObj->id);

	addBoolVarToGroup(strBuff, "isVisible",  &spObj->_isVisible);
	addBoolVarToGroup(strBuff, "isActive", &spObj->_isActive);
	addBoolVarToGroup(strBuff, "isInteracting", &spObj->_isInteracting);
	if(ableToSetTextured) addBoolVarToGroup(strBuff, "isTextured", &spObj->_isTextured);

	addColorVarToGroup(strBuff, "color", &spObj->color);

	if (spObj->getType() == STAR) {
		Star * star = dynamic_cast<Star *>(spObj);
		addDoubleVarToGroup(strBuff, "linearAttentuation(abstactMetrics)", &star->linearAtten, true);
		addDoubleVarToGroup(strBuff, "quadraticAttentuation(abstactMetrics)", &star->quadraticAtten, true);
	}

	addDoubleVarToGroup(strBuff, "rotationAnglePerYear", &spObj->rotationAnglePerYear, false, -5, 3);
	addDoubleVarToGroup(strBuff, "mass", &spObj->mass, true, -5, 40);
	addDoubleVarToGroup(strBuff, "diameter", &spObj->diameter, true, -5, 10);
	
	addVector3dVarToGroup(strBuff, "position", &spObj->position, false, -100, 14);
	addVector3dVarToGroup(strBuff, "velocity", &spObj->velocity, false, -100, 10);
	addVector3dVarToGroup(strBuff, "gravityAcceleration", &spObj->gravityAccel, false, -100, 100, false, true);
	addVector3dVarToGroup(strBuff, "additionalAcceleration", &spObj->additionalAccel, false, -100, 10);
	addVector3dVarToGroup(strBuff, "resultAcceleration", &spObj->resultAccel, false, -100, 100, false, true);

	if (spObj->getType() == PLANET) {
		Planet * planet = dynamic_cast<Planet *>(spObj);
		sprintf(strBuff1, "%s_ring_rot", strBuff);
		sprintf(strBuff2, "label='rot' group=%s_ring", strBuff);
		//TwAddVarRW(twBar, strBuff1, TW_TYPE_QUAT4F, &planet->ring.quat, strBuff2);
		sprintf(strBuff1, "%s_ring", strBuff);
		addBoolVarToGroup(strBuff1, "isTextured", &planet->ring.isTextured);
		addDoubleVarToGroup(strBuff1, "diameter", &planet->ring.diameter, true, -5, 10);
		addDoubleVarToGroup(strBuff1, "width", &planet->ring.width, true, -5, 10);
		addColorVarToGroup(strBuff1, "color", &planet->ring.color);

		moveOneGroupToAnother(strBuff1, strBuff);
		sprintf(strBuff1, "'%s'/%s_ring label='ring'", TwGetBarName(twBar), strBuff);
		TwDefine(strBuff1);
	}

	SpaceObjectAndPane * spObjAndPane = new SpaceObjectAndPane(spObj, this);

	if (ableToSetDefault) {
		void * v;
		if (spObj->getType() == STAR) {
			Star * star = dynamic_cast<Star *>(spObj);
			v = new VarWithDefault<Star>(star, *star);
			addButtonToGroup(strBuff, "setDefault", TwCB::setDefaultStar_onButtonClick, v);
		}
		else if (spObj->getType() == PLANET) {
			Planet * planet = dynamic_cast<Planet *>(spObj);
			v = new VarWithDefault<Planet>(planet, *planet);
			addButtonToGroup(strBuff, "setDefault", TwCB::setDefaultPlanet_onButtonClick, v);
		}
		else if (spObj->getType() == SPUTNIK) {
			Sputnik * sputnik = dynamic_cast<Sputnik *>(spObj);
			v = new VarWithDefault<Sputnik>(sputnik, *sputnik);
			addButtonToGroup(strBuff, "setDefault", TwCB::setDefaultSputnik_onButtonClick, v);
		}
		else if (spObj->getType() == TEAPOT) {
			Teapot * teapot = dynamic_cast<Teapot *>(spObj);
			v = new VarWithDefault<Teapot>(teapot, *teapot);
			addButtonToGroup(strBuff, "setDefault", TwCB::setDefaultTeapot_onButtonClick, v);
		}
		spObjsWithDefault.push_back(v);
		bufferList.push_back(v);
	}

	if (ableToDelete) {
		addButtonToGroup(strBuff, "delete", TwCB::deleteSpaceObject_onButtonClick, spObjAndPane);
	}

	if (closed) {
		sprintf(strBuff1, "'%s'/%s opened=false", TwGetBarName(twBar), strBuff);
		TwDefine(strBuff1);
	}

	sprintf(strBuff1, "'%s'/%s label='%s'", TwGetBarName(twBar), strBuff, spObj->getName());
	TwDefine(strBuff1);
}

void Pane::removeSpaceObject(SpaceObject * spObj)
{
	char strBuff[256];

	sprintf(strBuff, "spaceObject#%d_name", spObj->id);
	TwRemoveVar(twBar, strBuff);

	sprintf(strBuff, "spaceObject#%d_isVisible", spObj->id);
	TwRemoveVar(twBar, strBuff);

	sprintf(strBuff, "spaceObject#%d_isActive", spObj->id);
	TwRemoveVar(twBar, strBuff);

	sprintf(strBuff, "spaceObject#%d_isInteracting", spObj->id);
	TwRemoveVar(twBar, strBuff);

	sprintf(strBuff, "spaceObject#%d_color", spObj->id);
	TwRemoveVar(twBar, strBuff);

	sprintf(strBuff, "spaceObject#%d_mass", spObj->id);
	removeDoubleVar(strBuff);

	sprintf(strBuff, "spaceObject#%d_diameter", spObj->id);
	removeDoubleVar(strBuff);

	sprintf(strBuff, "spaceObject#%d_velocity", spObj->id);
	removeVector3dVar(strBuff);

	sprintf(strBuff, "spaceObject#%d_gravityAcceleration", spObj->id);
	removeVector3dVar(strBuff);

	sprintf(strBuff, "spaceObject#%d_additionalAcceleration", spObj->id);
	removeVector3dVar(strBuff);

	sprintf(strBuff, "spaceObject#%d_resultAcceleration", spObj->id);
	removeVector3dVar(strBuff);

	sprintf(strBuff, "spaceObject#%d_position", spObj->id);
	removeVector3dVar(strBuff);

	sprintf(strBuff, "spaceObject#%d_setDefault", spObj->id);
	TwRemoveVar(twBar, strBuff);

	sprintf(strBuff, "spaceObject#%d_delete", spObj->id);
	TwRemoveVar(twBar, strBuff);

	sprintf(strBuff, "spaceObject#%d_isTextured", spObj->id);
	TwRemoveVar(twBar, strBuff);

	sprintf(strBuff, "spaceObject#%d_createSputnik", spObj->id);
	TwRemoveVar(twBar, strBuff);

	sprintf(strBuff, "spaceObject#%d_nextTexture", spObj->id);
	TwRemoveVar(twBar, strBuff);

	sprintf(strBuff, "spaceObject#%d_rotationAnglePerYear", spObj->id);
	TwRemoveVar(twBar, strBuff);

	sprintf(strBuff, "spaceObject#%d_linearAttentuation(abstactMetrics)", spObj->id);
	TwRemoveVar(twBar, strBuff);

	sprintf(strBuff, "spaceObject#%d_quadraticAttentuation(abstactMetrics)", spObj->id);
	TwRemoveVar(twBar, strBuff);

	sprintf(strBuff, "spaceObject#%d_ring_rot", spObj->id);
	TwRemoveVar(twBar, strBuff);

	sprintf(strBuff, "spaceObject#%d_ring_isTextured", spObj->id);
	TwRemoveVar(twBar, strBuff);

	sprintf(strBuff, "spaceObject#%d_ring_diameter", spObj->id);
	TwRemoveVar(twBar, strBuff);

	sprintf(strBuff, "spaceObject#%d_ring_width", spObj->id);
	TwRemoveVar(twBar, strBuff);

	sprintf(strBuff, "spaceObject#%d_ring_color", spObj->id);
	TwRemoveVar(twBar, strBuff);
}

void Pane::show()
{
	char strBuff[256];

	sprintf(strBuff, "'%s' visible=true", TwGetBarName(twBar));
	TwDefine(strBuff);
	_isVisible = true;
}


void Pane::hide()
{
	char strBuff[256];

	sprintf(strBuff, "'%s' visible=false", TwGetBarName(twBar));
	TwDefine(strBuff);
	_isVisible = false;
}

Pane::~Pane()
{
	for each (void * v in bufferList) {
		delete v;
	}
}

//

ControlPane * ControlPane::getInstance(Camera * camera, RenderManager * renderManager)
{
	if (NULL != instance) return NULL;

	instance = new ControlPane(camera, renderManager);

	return instance;
}

ControlPane::ControlPane(Camera * camera, RenderManager * renderManager) {
	twBar = TwNewBar("Control pane");
	TwDefine(" 'Control pane' contained=true size='300 600' color='0 0 20' position='0 0' alpha=0 label='Control pane (SI metrics)' ");
	hide();
	newSpObjPane = NewSpaceObjectPane::getInstance(this, renderManager);
	this->camera = camera;

	TwAddVarRW(twBar, "cameraRot", TW_TYPE_QUAT4F, camera->quat, "opened=true");
	VarWithDefault<Vector3f> * v = new VarWithDefault<Vector3f>((Vector3f *)camera->quat, *(Vector3f *)(camera->quat));
	TwAddButton(twBar, "cameraRot_setDefault", TwCB::setDefaultVector3f_onButtonClick, v, "label='setDefault'");
	moveOneGroupToAnother("cameraRot_setDefault", "cameraRot");
	TwAddSeparator(twBar, "rot[Sep]", "");

	//TwAddSeparator(twBar, "setDefaultSystem[Sep]", "");
	addDoubleVarToGroup("factors", "G", &SpaceObject::G, false);
	addDoubleVarToGroup("factors", "timeAcceleration", &SpaceObject::timeAccel, false, 0, 6);
	//addDoubleVarToGroup("factors", "distanceCompression", &SpaceObject::metersPerPixel, true, 9, 9);
	addDoubleVarToGroup("factors", "starsDiameterDisplayFactor", &Star::diamDisplayFactor, true, 0, 1);
	addDoubleVarToGroup("factors", "planetsDiameterDisplayFactor", &Planet::diamDisplayFactor, true, 0, 2);
	addDoubleVarToGroup("factors", "sputniksDiameterDisplayFactor", &Sputnik::diamDisplayFactor, true, 0, 2);
	addDoubleVarToGroup("factors", "sputniksDistanceToParentDisplayFactor", &Sputnik::distDisplayFactor, true, 0, 2);
	addDoubleVarToGroup("factors", "teapotsDiameterDisplayFactor", &Teapot::diamDisplayFactor, true, 0, 10);
	TwAddSeparator(twBar, "factors[Sep]", "");

	TwAddButton(twBar, "createStar", TwCB::showNewSpaceObjectPane_star_onButtonClick, this->newSpObjPane, "label='createStar'");
	TwAddButton(twBar, "createPlanet", TwCB::showNewSpaceObjectPane_planet_onButtonClick, this->newSpObjPane, "label='createPlanet'");
	TwAddSeparator(twBar, "showNewSpaceObjectPane[Sep]", "");

	SpaceObjectAndCamera * spObjAndCam = new SpaceObjectAndCamera(NULL, this->camera);
	bufferList.push_back(spObjAndCam);
	TwAddButton(twBar, "setDefaultCameraTarget", TwCB::setCameraTarget_onButtonClick, spObjAndCam, "");
	TwAddButton(twBar, "setAllDefault", TwCB::setDefaultSpaceObjectsInList_onButtonClick, &spObjsWithDefault, "");
	TwAddSeparator(twBar, "setDefault[Sep]", "");
}

void ControlPane::addSpaceObject(SpaceObject * spObj, bool ableToSetTextured, bool ableToSetDefault, bool ableToDelete, bool closed) // adding to categories
{
	char strBuff[256], strBuff1[256];

	SpaceObjectAndCamera * spObjAndCam = new SpaceObjectAndCamera(spObj, this->camera);

	sprintf(strBuff, "spaceObject#%d", spObj->id);
	addButtonToGroup(strBuff, "setCameraTarget", TwCB::setCameraTarget_onButtonClick, spObjAndCam);
	
	if (spObj->getType() == PLANET) {
		SpaceObjectAndPane * newSpObjAndPane = new SpaceObjectAndPane(spObj, this->newSpObjPane);
		addButtonToGroup(strBuff, "createSputnik", TwCB::showNewSpaceObjectPane_sputnik_onButtonClick, newSpObjAndPane);
		addButtonToGroup(strBuff, "nextTexture", TwCB::changePlanetTexture_onButtonClick, spObj);
	}

	Pane::addSpaceObject(spObj, ableToSetTextured, ableToSetDefault, ableToDelete, closed);

	if (spObj->getType() == SPUTNIK) {
		SpaceObject * parent = spObj->getParent();

		sprintf(strBuff, "spaceObject#%d", spObj->getId());
		sprintf(strBuff1, "spaceObject#%d_sputniks", parent->getId());
		moveOneGroupToAnother(strBuff, strBuff1);

		sprintf(strBuff, "'Control pane'/%s label='sputniks' opened=false", strBuff1);
		TwDefine(strBuff);

		sprintf(strBuff, "spaceObject#%d", parent->getId());
		moveOneGroupToAnother(strBuff1, strBuff);
	}else {
		sprintf(strBuff, "spaceObject#%d", spObj->getId());
		sprintf(strBuff1, "%ss", spObj->getTypeStr());
		moveOneGroupToAnother(strBuff, strBuff1);
	}
}

void ControlPane::removeSpaceObject(SpaceObject * spObj) {
	Pane::removeSpaceObject(spObj);
	
	char strBuff[256];
	sprintf(strBuff, "spaceObject#%d_setCameraTarget", spObj->id);
	TwRemoveVar(twBar, strBuff);

	sprintf(strBuff, "spaceObject#%d_createSputnik", spObj->id);
	TwRemoveVar(twBar, strBuff);
}

void ControlPane::showNewSpaceObjectPane()
{
	newSpObjPane->show();
}

void ControlPane::hideNewSpaceObjectPane()
{
	newSpObjPane->hide();
}

ControlPane::~ControlPane() {
	delete newSpObjPane;
}

NewSpaceObjectPane * NewSpaceObjectPane::getInstance(ControlPane * ctrlPane, RenderManager * renderManager)
{
	if (NULL != instance) return NULL;

	instance = new NewSpaceObjectPane(ctrlPane, renderManager);

	return instance;
}

NewSpaceObjectPane::NewSpaceObjectPane(ControlPane * ctrlPane, RenderManager * renderManager) {
	twBar = TwNewBar("New space object pane");
	TwDefine(" 'New space object pane' contained=true size='300 600' color='0 0 20' position='2000 0' alpha=0 label='New space object pane (SI metrics)'");
	hide();
	this->ctrlPane = ctrlPane;
	this->renderManager = renderManager;
}

void  NewSpaceObjectPane::addNewSpaceObject(SpaceObjectType spObjType, SpaceObject * parent) {
	if (spObjType == PLANET) {
		newSpObj = new Planet("Mars", 6.4e23, 6.7e6, Vector3f(1.0f, 0.0f, 0.0f), Vector3d(2.28e11, 0.0, 0.0), Vector3d(0.0, 2.4e4, 0.0));
		newSpObj->loadTexture("textures/mars.tga");
	}
	else if (spObjType == STAR) {
		newSpObj = new Star("Sun", 2.0e30, 1.4e9, Vector3f(1.0, 1.0, 0.0), Vector3d(0.0, 0.0, 2.0e11), Vector3d(0.0, sqrt(SpaceObject::G * 2.0e30 / 1.0e11), 0.0));
		newSpObj->loadTexture("textures/sun.tga");
	}
	else if (spObjType == SPUTNIK) {
		Planet * parPlanet = dynamic_cast<Planet *>(parent);
		Vector3d position(4.0e8 * pow(parPlanet->mass/6.0e24, 0.35), 0.0, 0.0); // counting conviniant pos
		Vector3d velocity;
		velocity.y = sqrt( SpaceObject::G * parPlanet->mass / position.x);

		newSpObj = new Sputnik("Moon", 7.35e22, 3.4e6, Vector3f(0.7f, 0.7f, 0.7f), dynamic_cast<Planet *>(parent), position, velocity);
		newSpObj->loadTexture("textures/moon.tga");
	}
	addSpaceObject(newSpObj, true, false, false);

	TwAddSeparator(twBar, "confirm[Sep]", "");
	TwAddButton(twBar, "confirm", TwCB::createSpaceObject_onButtonClick, this, "label='confirm'");
	TwAddButton(twBar, "cancel", TwCB::hideNewSpaceObjectPane_onButtonClick, this, "label='cancel'");
}

void NewSpaceObjectPane::hide(){
	if(newSpObj != NULL){
		removeSpaceObject(newSpObj);
		newSpObj = NULL;
	}
	TwRemoveVar(twBar, "confirm[Sep]");
	TwRemoveVar(twBar, "confirm");
	TwRemoveVar(twBar, "cancel");
	Pane::hide();
}

void NewSpaceObjectPane::addSpaceObject(SpaceObject * spObj, bool ableToSetDefault, bool ableToDelete, bool closed)
{
	char strBuff[256];

	sprintf(strBuff, "spaceObject#%d", spObj->id);
	addStrVarToGroup(strBuff, "name", &spObj->name);

	Pane::addSpaceObject(spObj, true, ableToSetDefault, ableToDelete, closed);

	sprintf(strBuff, "'%s'/'spaceObject#%d' label='new'", TwGetBarName(twBar), spObj->id);
	TwDefine(strBuff);
}

void NewSpaceObjectPane::createSpaceObject()
{
	renderManager->renderSpaceObject(newSpObj);

	ctrlPane->addSpaceObject(newSpObj, false);
	removeSpaceObject(newSpObj);
}

NewSpaceObjectPane::~NewSpaceObjectPane()
{
	delete newSpObj;
}
