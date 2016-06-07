#include "stdafx.h"

#include <math.h>
#include <GL/freeglut.h> 
#include <iostream>

#include "utils.h"
#include "camera.h"
#include "constants.h"

const float Camera::maxDistance = 1000.0f; // in abstract pixels

Camera::Camera(const float initialDistance) {
	init(initialDistance);
}

void Camera::init(const float initialDistance) {
	dist = initialDistance;
	
	quat[0] = quat[1] = quat[2] = 0.0f;
	quat[3] = 1.0f;

	pos.x = pos.y = pos.z = 0.0f;
};

void Camera::render() {

	float mat[16], mat1[16];
	
	glLoadIdentity();
	glGetFloatv(GL_MODELVIEW_MATRIX, mat1);
	Utils::getModifMat(quat, mat1, mat);
	Vector3d target, targetPix;
	if (targetSpObj != NULL) {
		SpaceObject * parent = targetSpObj->getParent();
		if (parent != NULL) {
			target = parent->getPosition() + targetSpObj->getPosition()*(targetSpObj->getType() == SPUTNIK ? dynamic_cast<Sputnik *>(targetSpObj)->getDistDisplayFactor() : 1.0);
		}
		else {
			target = targetSpObj->getPosition();
		}

		targetPix = target * (1 / SpaceObject::getMetersPerPixel());
 		if (targetPix.length() > Camera::maxDistance / 2) {
			targetSpObj = NULL;
			target.nullify();
		}
	}
	if (dist < 0.03f) dist = 0.03f;
	glTranslatef(0.0f, 0.0f, -dist);
	glMultMatrixf(mat);
	glTranslatef(-targetPix.x, -targetPix.y, -targetPix.z);
}

void Camera::zoom(float deltaP) {
	double minDist = (targetSpObj != NULL) ? (targetSpObj->getDiameter() * targetSpObj->getDiamDisplayFactor() * 1.6 / SpaceObject::getMetersPerPixel()) : 0.0;
	if ((dist < minDist && deltaP < 0.0f) || (dist > maxDistance && deltaP > 0.0f)) return;
	/*Vector3d pos;
	SpaceObject * target = targetSpObj;
	if(target != NULL){
		pos = target->getPosition() + 
	}
	if ((dist > Camera::maxDistance && delta > 0.0f)*/
	dist += deltaP*dist;
}

void Camera::setTarget(SpaceObject * spObj){
	targetSpObj = spObj;
	
	double minDist = (targetSpObj != NULL) ? (targetSpObj->getDiameter() * targetSpObj->getDiamDisplayFactor() * 1.6 / SpaceObject::getMetersPerPixel()) : 0.0f;
	if (dist < minDist) dist = minDist;
}
	