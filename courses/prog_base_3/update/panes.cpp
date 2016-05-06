#include "stdafx.h"

#include <stdio.h>

#include "panes.h"

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

	sprintf(strBuff1, "'%s'/%s opened=false label='%s'", TwGetBarName(twBar), strBuff, varName);
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

void Pane::addSpaceObject(SpaceObject * spObj, bool ableToSetDefault)
{
	char strBuff[256], strBuff1[256];
	sprintf(strBuff, "spaceObject#%d", spObj->id);
	addStrVarToGroup(strBuff, "name", &spObj->name);

	addBoolVarToGroup(strBuff, "isVisible", &spObj->_isVisible);
	addBoolVarToGroup(strBuff, "isActive", &spObj->_isActive);
	addBoolVarToGroup(strBuff, "isInteracting", &spObj->_isInteracting);

	addColorVarToGroup(strBuff, "color", &spObj->color);
	addDoubleVarToGroup(strBuff, "mass", &spObj->mass, true);
	addDoubleVarToGroup(strBuff, "diameter", &spObj->diameter, true);

	addVector3dVarToGroup(strBuff, "velocity", &spObj->velocity, false);
	addVector3dVarToGroup(strBuff, "acceleration", &spObj->resultAccel, false);
	addVector3dVarToGroup(strBuff, "position", &spObj->position, false);

	SpaceObjectAndPane * spObjAndPane = new SpaceObjectAndPane();
	spObjAndPane->spObj = spObj;
	spObjAndPane->pane = this;

	if (ableToSetDefault) {
		void * v;
		if (spObj->getType() == STAR) {
			Star * star = dynamic_cast<Star *>(spObj);
			v = new VarWithDefault<Star>(dynamic_cast<Star *>(star), *star);
			addButtonToGroup(strBuff, "setDefault", TwCB::setDefaultStar_onButtonClick, v);
		}
		else if (spObj->getType() == PLANET) {
			Planet * planet = dynamic_cast<Planet *>(spObj);
			v = new VarWithDefault<Planet>(dynamic_cast<Planet *>(planet), *planet);
			addButtonToGroup(strBuff, "setDefault", TwCB::setDefaultPlanet_onButtonClick, v);
		}
		else if (spObj->getType() == SPUTNIK) {
			Sputnik * sputnik = dynamic_cast<Sputnik *>(spObj);
			v = new VarWithDefault<Sputnik>(dynamic_cast<Sputnik *>(sputnik), *sputnik);
			addButtonToGroup(strBuff, "setDefault", TwCB::setDefaultSputnik_onButtonClick, v);
		}
	}
	addButtonToGroup(strBuff, "delete", TwCB::deleteSpaceObject_onButtonClick, spObjAndPane);

	sprintf(strBuff1, "'%s'/%s opened=false label='%s'", TwGetBarName(twBar), strBuff, spObj->getName());
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

Pane::~Pane()
{
	for each (void * v in bufferList) {
		delete v;
	}
}

//

ControlPane * ControlPane::getInstance()
{
	if (NULL != instance) return NULL;
	
	instance = new ControlPane();

	instance->twBar = TwNewBar("Control pane");
	instance->init();
	return instance;
}

void ControlPane::init() {
	TwDefine(" 'Control pane' contained=true size='300 600' color='0 0 20' position='0 0' alpha=0 label='Control pane (SI metrics)' ");
	TwAddVarRW(twBar, "rot", TW_TYPE_QUAT4F, Camera::quat, "opened=true");
	TwAddSeparator(twBar, "rotSep", "");
	//wAddButton(p_instance->twBar, "factors", _TwCB_callProc, SpaceObject::setDefaultFactors, "");
	addDoubleVarToGroup("factors", "G", &SpaceObject::G, false);
	addDoubleVarToGroup("factors", "timeAcceleration", &SpaceObject::timeAccel, false);
	addDoubleVarToGroup("factors", "distanceExtention", &SpaceObject::metersPerPixel, true);
	addDoubleVarToGroup("factors", "starsDiameterDisplayFactor", &Star::diamDisplayFactor, true);
	addDoubleVarToGroup("factors", "planetsDiameterDisplayFactor", &Planet::diamDisplayFactor, true);
	addDoubleVarToGroup("factors", "sputniksDiameterDisplayFactor", &Sputnik::diamDisplayFactor, true);
	addDoubleVarToGroup("factors", "sputniksDistanceToParentDisplayFactor", &Sputnik::distDisplayFactor, true);
	TwAddSeparator(twBar, "factorsSep", "");

}

void ControlPane::addSpaceObject(SpaceObject * spObj) // adding to categories
{
	char strBuff[256], strBuff1[256];

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
	}else {
		sprintf(strBuff, "spaceObject#%d", spObj->getId());
		sprintf(strBuff1, "%ss", spObj->getTypeStr());
		moveOneGroupToAnother(strBuff, strBuff1);

	}
}

NewSpaceObjectPane * NewSpaceObjectPane::getInstance(ControlPane * ctrlPane)
{
	if (NULL != instance) return NULL;

	instance = new NewSpaceObjectPane();

	instance->twBar = TwNewBar("New space object pane");
	instance->init(ctrlPane);
	return instance;
}

void TW_CALL setSpObj(void * clientData) {
	SpaceObject * newSpObj = static_cast<Planet *>(clientData);
	RenderManager::addSpaceObjectForRendering(newSpObj);
}

void NewSpaceObjectPane::init(ControlPane * ctrlPane) {
	TwDefine(" 'New space object pane' contained=true size='300 600' color='0 0 20' position='2000 0' alpha=0 label='New space object pane (SI metrics)'");
	this->ctrlPane = ctrlPane;
	newSpObj = new Planet("new", 0.0, 0.0, Vector3f(0.0, 0.0, 0.0), Vector3d(0.0, 0.0, 0.0), Vector3d(0.0, 0.0, 0.0));
	Pane::addSpaceObject(newSpObj);

	TwAddButton(twBar, "confirm", TwCB::createSpaceObject_onButtonClick, this, "");
}

void NewSpaceObjectPane::createSpaceObject()
{
	printf(newSpObj->getName());
	ctrlPane->addSpaceObject(newSpObj);
	RenderManager::addSpaceObjectForRendering(newSpObj);

	newSpObj = new Planet("new", 0.0, 0.0, Vector3f(0.0, 0.0, 0.0), Vector3d(0.0, 0.0, 0.0), Vector3d(0.0, 0.0, 0.0));
	Pane::addSpaceObject(newSpObj);
}

NewSpaceObjectPane::~NewSpaceObjectPane()
{
	delete newSpObj;
}
