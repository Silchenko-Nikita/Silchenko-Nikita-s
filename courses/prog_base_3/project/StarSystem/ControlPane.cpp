#include "stdafx.h"

#include "SpaceObjects.h"
#include "ControlPane.h"
#include "utils.h"

ControlPane * ControlPane::p_instance;

static void TW_CALL  _TwCB_spObj_setMantissa(const void *value, void *clientData) {
	double * spObj_field = static_cast<double *>(clientData);
	int exp = 0;

	Utils::getMantissaAndExp(*spObj_field, &exp);

	*spObj_field = *static_cast<const double *>(value) * pow(10, exp);
}

static void TW_CALL _TwCB_spObj_getMantissa(void *value, void *clientData) {
	double * spObj_field = static_cast<double *>(clientData);
	double mantissa;

	mantissa = Utils::getMantissaAndExp(*spObj_field, NULL);

	*static_cast<double *>(value) = mantissa;
}


static void TW_CALL _TwCB_spObj_setExp(const void *value, void *clientData) {
	double * spObj_field = static_cast<double *>(clientData);
	double mantissa;

	mantissa = Utils::getMantissaAndExp(*spObj_field, NULL);

	*spObj_field = mantissa * pow(10, *static_cast<const int *>(value));
}

static void TW_CALL _TwCB_spObj_getExp(void *value, void * clientData) {
	double * spObj_field = static_cast<double *>(clientData);
	int exp = 0;

	Utils::getMantissaAndExp(*spObj_field, &exp);

	*static_cast<int *>(value) = exp;
}

static void _TwUtil_moveOneGroupToAnother(TwBar * twBar, const char * moving, const char * target)
{
	char strBuff[256];
	sprintf(strBuff, "'%s'/%s group= %s", TwGetBarName(twBar), moving, target);

	TwDefine(strBuff);
}


static void _TwUtil_addDoubleVar(TwBar * twBar, const char * varName,  double * var)
{
	char strBuff[256], strBuff1[256];

	sprintf(strBuff, "%s_mantissa", varName);
	sprintf(strBuff1, "label='mantissa' group='%s' min=0.0 max=9.9 step=0.02", varName);
	TwAddVarCB(twBar, strBuff, TW_TYPE_DOUBLE, _TwCB_spObj_setMantissa, _TwCB_spObj_getMantissa, var, strBuff1);

	sprintf(strBuff, "%s_exponent", varName);
	sprintf(strBuff1, "label='exponent' group='%s'", varName);
	TwAddVarCB(twBar, strBuff, TW_TYPE_INT32, _TwCB_spObj_setExp, _TwCB_spObj_getExp, var, strBuff1);
}

static void _TwUtil_addSpObjDoubleField(TwBar * twBar, SpaceObject * spObj, const char * fieldName, double * field)
{
	char strBuff[256], strBuff1[256];

	sprintf(strBuff, "%s_%s", spObj->getName(), fieldName);
	_TwUtil_addDoubleVar(twBar, strBuff, field);
	sprintf(strBuff1, "'Control pane'/%s label='%s' opened=false", strBuff, fieldName);
	TwDefine(strBuff1);
	_TwUtil_moveOneGroupToAnother(twBar, strBuff, spObj->getName());
}

static void _TwUtil_addSpObjColorField(TwBar * twBar, SpaceObject * spObj, const char * fieldName, Vector3d * field)
{
/*	char strBuff[256], strBuff1[256];

	sprintf(strBuff, "%s_%s", spObj->getName(), fieldName);
	TwAddVarRW(p_instance->twBar, strBuff, TW_TYPE_COLOR3F, spObj->,);
	TwAddVarRW(,,,,,);

	sprintf(strBuff1, "'Control pane'/%s label='%s'", strBuff, fieldName);
	TwDefine(strBuff1);
	_TwUtil_moveOneGroupToAnother(twBar, strBuff, spObj->getName());*/
}

ControlPane * ControlPane::getInstance()
{
	if (NULL != p_instance) return NULL;
	
	p_instance = new ControlPane();

	p_instance->twBar = TwNewBar("Control pane");
	TwDefine(" 'Control pane' contained=true ");
	TwDefine(" 'Control pane' size='300 400' color='100 200 200' ");
	TwDefine(" 'Control pane' label='Control pane (SI metrics)' ");

	return p_instance;
}

void ControlPane::addSpObj(SpaceObject * spObj)
{
	char strBuff[256];

	sprintf(strBuff, "%s_%s", spObj->getName(), "color");
	TwAddVarRW(p_instance->twBar, strBuff, TW_TYPE_COLOR3F, (void *)&spObj->color, "");
	_TwUtil_moveOneGroupToAnother(twBar, strBuff, spObj->getName());
	sprintf(strBuff, "'Control pane'/%s opened=false", spObj->getName());
	TwDefine(strBuff);

	sprintf(strBuff, "'Control pane'/%s_color ", spObj->getName());
	TwDefine(strBuff);

	_TwUtil_addSpObjDoubleField(twBar, spObj, "mass", &spObj->mass);
	_TwUtil_addSpObjDoubleField(twBar, spObj, "diameter", &spObj->diameter);

	if (spObj->getType() == SPUTNIK) {
		SpaceObject * parent = spObj->getParent();

		sprintf(strBuff, "%s_sputniks", parent->getName());
		_TwUtil_moveOneGroupToAnother(p_instance->twBar, spObj->getName(), strBuff);

		sprintf(strBuff, "'Control pane'/%s_sputniks label='sputniks' opened=false", parent->getName());
		TwDefine(strBuff);

		sprintf(strBuff, "%s_sputniks", parent->getName());
		_TwUtil_moveOneGroupToAnother(p_instance->twBar, strBuff, parent->getName());
	}else {
		sprintf(strBuff, "%ss", spObj->getTypeStr());
		_TwUtil_moveOneGroupToAnother(p_instance->twBar, spObj->getName(), strBuff);

	}
}

ControlPane::~ControlPane()
{
	TwDeleteBar(p_instance->twBar);
}