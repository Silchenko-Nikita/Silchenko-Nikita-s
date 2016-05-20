
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
	sprintf(strBuff, "'%s'/%s group= %s", TwGetBarName(twBar), moving, target);
	TwDefine(strBuff);
}

void Pane::addDoubleVar(const char * varName, double * var, bool isNonnegat, bool ableToSetDefault)
{
	char strBuff[256], strBuff1[256];

	sprintf(strBuff, "%s_mantissa", varName);
	sprintf(strBuff1, "label='mantissa' group='%s' min=%s max=10.0 step=0.02", varName, isNonnegat ? "0.0" : "-10.0");
	TwAddVarCB(twBar, strBuff, TW_TYPE_DOUBLE, TwCB::setMantissa, TwCB::getMantissa, var, strBuff1);

	sprintf(strBuff, "%s_exponent", varName);
	sprintf(strBuff1, "label='exponent' group='%s'", varName);
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

void Pane::addVector3dVar(const char * varName, Vector3d * var, bool isNonnegat, bool ableToSetDefault) {
	char strBuff[256], strBuff1[256];

	addDoubleVarToGroup(varName, "x", &var->x, isNonnegat, ableToSetDefault);
	addDoubleVarToGroup(varName, "y", &var->y, isNonnegat, ableToSetDefault);
	addDoubleVarToGroup(varName, "z", &var->z, isNonnegat, ableToSetDefault);

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

void Pane::addDoubleVarToGroup(const char * groupName, const char * varName, double * var, bool isNonnegat, bool ableToSetDefault)
{
	char strBuff[256], strBuff1[256];

	sprintf(strBuff, "%s_%s", groupName, varName);
	addDoubleVar(strBuff, var, isNonnegat, ableToSetDefault);
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

void Pane::addVector3dVarToGroup(const char * groupName, const char * varName, Vector3d * var, bool isNonnegat, bool ableToSetDefault)
{
	char strBuff[256], strBuff1[256];

	sprintf(strBuff, "%s_%s", groupName, varName);
	addVector3dVar(strBuff, var, isNonnegat, ableToSetDefault);

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

	/*if (ableToSetDefault) {
	StrAndDefault * v = new StrAndDefault();
	v->var = var;
	v->defaultVal = *var;

	sprintf(strBuff, "%s_setDefault", strBuff);
	sprintf(strBuff1, "label='setDefault' group='%s'", varName);
	TwAddButton(twBar, strBuff, TwCB::setDefaultStr, v, strBuff1);
	}*/
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

void Pane::addSpaceObject(SpaceObject * spObj, bool ableToSetDefault, bool ableToDelete, bool closed)
{
	char strBuff[256], strBuff1[256];

	sprintf(strBuff, "spaceObject#%d", spObj->id);

	addBoolVarToGroup(strBuff, "isVisible", &spObj->_isVisible);
	addBoolVarToGroup(strBuff, "isActive", &spObj->_isActive);
	addBoolVarToGroup(strBuff, "isInteracting", &spObj->_isInteracting);

	addColorVarToGroup(strBuff, "color", &spObj->color);
	addDoubleVarToGroup(strBuff, "mass", &spObj->mass, true);
	addDoubleVarToGroup(strBuff, "diameter", &spObj->diameter, true);

	addVector3dVarToGroup(strBuff, "position", &spObj->position, false);
	addVector3dVarToGroup(strBuff, "velocity", &spObj->velocity, false);
	addVector3dVarToGroup(strBuff, "acceleration", &spObj->resultAccel, false);

	SpaceObjectAndPane * spObjAndPane = new SpaceObjectAndPane(spObj, this);

	if (ableToSetDefault) {
		void * v;
		if (spObj->getType() == STAR) {
			Star * star = dynamic_cast<Star *>(spObj);
			v = new VarWithDefault<Star>(star, *star);
			spObjsWithDefault.push_back(v);
			addButtonToGroup(strBuff, "setDefault", TwCB::setDefaultStar_onButtonClick, v);
		}
		else if (spObj->getType() == PLANET) {
			Planet * planet = dynamic_cast<Planet *>(spObj);
			v = new VarWithDefault<Planet>(planet, *planet);
			spObjsWithDefault.push_back(v);
			addButtonToGroup(strBuff, "setDefault", TwCB::setDefaultPlanet_onButtonClick, v);
		}
		else if (spObj->getType() == SPUTNIK) {
			Sputnik * sputnik = dynamic_cast<Sputnik *>(spObj);
			v = new VarWithDefault<Sputnik>(sputnik, *sputnik);
			spObjsWithDefault.push_back(v);
			addButtonToGroup(strBuff, "setDefault", TwCB::setDefaultSputnik_onButtonClick, v);
		}
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

	sprintf(strBuff, "spaceObject#%d_acceleration", spObj->id);
	removeVector3dVar(strBuff);

	sprintf(strBuff, "spaceObject#%d_position", spObj->id);
	removeVector3dVar(strBuff);

	sprintf(strBuff, "spaceObject#%d_setDefault", spObj->id);
	TwRemoveVar(twBar, strBuff);

	sprintf(strBuff, "spaceObject#%d_delete", spObj->id);
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
	TwAddSeparator(twBar, "rot[Sep]", "");

	//TwAddSeparator(twBar, "setDefaultSystem[Sep]", "");
	addDoubleVarToGroup("factors", "G", &SpaceObject::G, false);
	addDoubleVarToGroup("factors", "timeAcceleration", &SpaceObject::timeAccel, false);
	addDoubleVarToGroup("factors", "distanceExtention", &SpaceObject::metersPerPixel, true);
	addDoubleVarToGroup("factors", "starsDiameterDisplayFactor", &Star::diamDisplayFactor, true);
	addDoubleVarToGroup("factors", "planetsDiameterDisplayFactor", &Planet::diamDisplayFactor, true);
	addDoubleVarToGroup("factors", "sputniksDiameterDisplayFactor", &Sputnik::diamDisplayFactor, true);
	addDoubleVarToGroup("factors", "sputniksDistanceToParentDisplayFactor", &Sputnik::distDisplayFactor, true);
	TwAddSeparator(twBar, "factors[Sep]", "");

	TwAddButton(twBar, "showNewSpaceObjectPane_star", TwCB::showNewSpaceObjectPane_star_onButtonClick, this->newSpObjPane, "label='create new star'");
	TwAddButton(twBar, "showNewSpaceObjectPane_planet", TwCB::showNewSpaceObjectPane_planet_onButtonClick, this->newSpObjPane, "label='create new planet'");
	TwAddSeparator(twBar, "showNewSpaceObjectPane[Sep]", "");

	SpaceObjectAndCamera * spObjAndCam = new SpaceObjectAndCamera(NULL, this->camera);
	bufferList.push_back(spObjAndCam);
	TwAddButton(twBar, "setDefaultCameraTarget", TwCB::setCameraTarget_onButtonClick, spObjAndCam, "");
	TwAddButton(twBar, "setAllDefault", TwCB::setDefaultSpaceObjectsInList_onButtonClick, &spObjsWithDefault, "");
	TwAddSeparator(twBar, "setDefault[Sep]", "");
}

void ControlPane::addSpaceObject(SpaceObject * spObj, bool ableToSetDefault, bool ableToDelete, bool closed) // adding to categories
{
	char strBuff[256], strBuff1[256];

	SpaceObjectAndCamera * spObjAndCam = new SpaceObjectAndCamera(spObj, this->camera);

	sprintf(strBuff, "spaceObject#%d", spObj->id);
	addButtonToGroup(strBuff, "setCameraTarget", TwCB::setCameraTarget_onButtonClick, spObjAndCam);
	
	Pane::addSpaceObject(spObj);

	if (spObj->getType() == SPUTNIK) {
		SpaceObject * parent = spObj->getParent();

		sprintf(strBuff, "spaceObject#%d", spObj->getId());
		sprintf(strBuff1, "spaceObject#%d_sputniks", parent->getId());
		moveOneGroupToAnother(strBuff, strBuff1);

		sprintf(strBuff, "'Control pane'/%s label='sputniks' opened=false", strBuff1);
		TwDefine(strBuff);

		sprintf(strBuff, "spaceObject#%d", parent->getId());
		moveOneGroupToAnother(strBuff1, strBuff);
	}
	else {
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

void  NewSpaceObjectPane::addNewSpaceObject(SpaceObjectType spObjType) {
	if (spObjType == PLANET) {
		newSpObj = new Planet("Earth", 6.0e24, 1.2e7, Vector3f(0.0f, 0.0f, 1.0f), Vector3d(1.5e11, 0.0, 0.0), Vector3d(0.0, 3.0e4, 0.0));
	}
	else if (spObjType == STAR) {
		newSpObj = new Star("Sun", 2.0e30, 1.4e9, Vector3f(1.0, 1.0, 0.0), Vector3d(0.0, 0.0, 0.0), Vector3d(0.0, 0.0, 0.0));
	}
	addSpaceObject(newSpObj, true, false, false);

	TwAddSeparator(twBar, "confirm[Sep]", "");
	TwAddButton(twBar, "confirm", TwCB::createSpaceObject_onButtonClick, this, "label='confirm'");
	TwAddButton(twBar, "cancel", TwCB::hideNewSpaceObjectPane_onButtonClick, this, "label='cancel'");
}

void NewSpaceObjectPane::hide(){
	if(newSpObj != NULL)
		removeSpaceObject(newSpObj);
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

	Pane::addSpaceObject(spObj, ableToSetDefault, ableToDelete, closed);
}

void NewSpaceObjectPane::createSpaceObject()
{
	renderManager->renderSpaceObject(newSpObj);

	ctrlPane->addSpaceObject(newSpObj);
	removeSpaceObject(newSpObj);
}

NewSpaceObjectPane::~NewSpaceObjectPane()
{
	delete newSpObj;
}
