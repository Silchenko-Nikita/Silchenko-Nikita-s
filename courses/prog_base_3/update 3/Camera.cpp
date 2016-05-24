#include "stdafx.h"

#include <math.h>
#include <GL/freeglut.h> 
#include <iostream>

#include "utils.h"
#include "camera.h"
#include "constants.h"

const float Camera::maxDistance = 1000.0f; // in abstract pixels

Camera::Camera() {
	init();
}

void Camera::init() {
	dist = maxDistance;
	
	quat[0] = quat[1] = quat[2] = 0.0f;
	quat[3] = 1.0f;

	pos.x = pos.y = pos.z = 0.0f;
};

void Camera::render() {

	float mat[16];
	float yy2 = 2.0f * quat[1] * quat[1];
	float xy2 = 2.0f * quat[0] * quat[1];
	float xz2 = 2.0f * quat[0] * quat[2];
	float yz2 = 2.0f * quat[1] * quat[2];
	float zz2 = 2.0f * quat[2] * quat[2];
	float wz2 = 2.0f * quat[3] * quat[2];
	float wy2 = 2.0f * quat[3] * quat[1];
	float wx2 = 2.0f * quat[3] * quat[0];
	float xx2 = 2.0f * quat[0] * quat[0];
	mat[0 * 4 + 0] = -yy2 - zz2 + 1.0f;
	mat[0 * 4 + 1] = xy2 + wz2;
	mat[0 * 4 + 2] = xz2 - wy2;
	mat[0 * 4 + 3] = 0.0f;
	mat[1 * 4 + 0] = xy2 - wz2;
	mat[1 * 4 + 1] = -xx2 - zz2 + 1.0f;
	mat[1 * 4 + 2] = yz2 + wx2;
	mat[1 * 4 + 3] = 0.0f;
	mat[2 * 4 + 0] = xz2 + wy2;
	mat[2 * 4 + 1] = yz2 - wx2;
	mat[2 * 4 + 2] = -xx2 - yy2 + 1.0f;
	mat[2 * 4 + 3] = 0.0f;
	mat[3 * 4 + 0] = mat[3 * 4 + 1] = mat[3 * 4 + 2] = 0;
	mat[3 * 4 + 3] = 1;
		
	glLoadIdentity();
	Vector3d target;
	if (targetSpObj != NULL) {
		SpaceObject * parent = targetSpObj->getParent();
		if (parent != NULL) {
			target = parent->getPosition() + targetSpObj->getPosition()*(targetSpObj->getType() == SPUTNIK ? Sputnik::getDistDisplayFactor() : 1.0);
		}
		else {
			target = targetSpObj->getPosition();
		}
	}
	glTranslatef(0.0f, 0.0f, -dist);
	glMultMatrixf(mat);
	glTranslatef(-target.x / SpaceObject::getMetersPerPixel(), -target.y / SpaceObject::getMetersPerPixel(), -target.z / SpaceObject::getMetersPerPixel());
}

void Camera::zoom(float deltaP) {
	if ((dist < 0.0f && deltaP < 0.0f) || (dist > maxDistance && deltaP > 0.0f)) return;
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
}
	