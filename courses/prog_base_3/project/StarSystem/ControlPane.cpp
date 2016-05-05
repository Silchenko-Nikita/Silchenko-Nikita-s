#include "stdafx.h"

#include <stdio.h>

#include "SpaceObjects.h"
#include "ControlPane.h"
#include "twUtils.h"

ControlPane * ControlPane::p_instance;

ControlPane * ControlPane::getInstance()
{
	if (NULL != p_instance) return NULL;
	
	p_instance = new ControlPane();

	p_instance->twBar = TwNewBar("Control pane");
	p_instance->init();
	return p_instance;
}

static void TW_CALL _TwCB_callProc(void * clientData) {
	void (* proc)(void) = (void(*)(void))clientData;
	proc();
}

void ControlPane::init() {
	TwDefine(" 'Control pane' contained=true size='300 600' color='100 200 200' alpha=50 label='Control pane (SI metrics)' ");
	//wAddButton(p_instance->twBar, "factors", _TwCB_callProc, SpaceObject::setDefaultFactors, "");
	TwUtils::addDoubleVarToGroup(p_instance->twBar, "factors", "G", &SpaceObject::G, false);
	TwUtils::addDoubleVarToGroup(p_instance->twBar, "factors", "timeAcceleration", &SpaceObject::timeAccel, false);
	TwUtils::addDoubleVarToGroup(p_instance->twBar, "factors", "distanceExtention", &SpaceObject::metersPerPixel, true);
	TwUtils::addDoubleVarToGroup(p_instance->twBar, "factors", "starsDiameterDisplayFactor", &Star::diamDisplayFactor, true);
	TwUtils::addDoubleVarToGroup(p_instance->twBar, "factors", "planetsDiameterDisplayFactor", &Planet::diamDisplayFactor, true);
	TwUtils::addDoubleVarToGroup(p_instance->twBar, "factors", "sputniksDiameterDisplayFactor", &Sputnik::diamDisplayFactor, true);
	TwUtils::addDoubleVarToGroup(p_instance->twBar, "factors", "sputniksDistanceToParentDisplayFactor", &Sputnik::distDisplayFactor, true);

}

void ControlPane::addSpObj(SpaceObject * spObj)
{
	char strBuff[256];

	TwUtils::addBoolVarToGroup(twBar, spObj->getName(), "isInteracting", &spObj->isInteracting);
	TwUtils::addColorVarToGroup(twBar, spObj->getName(), "color", &spObj->color);
	TwUtils::addDoubleVarToGroup(twBar, spObj->getName(), "mass", &spObj->mass, true);
	TwUtils::addDoubleVarToGroup(twBar, spObj->getName(), "diameter", &spObj->diameter, true);
	TwUtils::addVector3dVarToGroup(twBar, spObj->getName(), "velocity", &spObj->velocity, false);
	TwUtils::addVector3dVarToGroup(twBar, spObj->getName(), "acceleration", &spObj->resultAccel, false);
	TwUtils::addVector3dVarToGroup(twBar, spObj->getName(), "position", &spObj->position, false);

	if (spObj->getType() == SPUTNIK) {
		SpaceObject * parent = spObj->getParent();

		sprintf(strBuff, "%s_sputniks", parent->getName());
		TwUtils::moveOneGroupToAnother(twBar, spObj->getName(), strBuff);

		sprintf(strBuff, "'Control pane'/%s_sputniks label='sputniks' opened=false", parent->getName());
		TwDefine(strBuff);

		sprintf(strBuff, "%s_sputniks", parent->getName());
		TwUtils::moveOneGroupToAnother(twBar, strBuff, parent->getName());
	}else {
		sprintf(strBuff, "%ss", spObj->getTypeStr());
		TwUtils::moveOneGroupToAnother(twBar, spObj->getName(), strBuff);

	}
}

ControlPane::~ControlPane()
{
	TwDeleteBar(twBar);
}