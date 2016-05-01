#include "stdafx.h"

#include "vector3.h"

Vector3d operator*(const Vector3d& v, const double k) {
	Vector3d res;
	res.x = v.x*k;
	res.y += v.y*k;
	res.z += v.z*k;
	return res;
}

Vector3d operator+=(Vector3d& v1, const Vector3d& v2) {
	v1.add(v2);
	return v1;
}

Vector3d::Vector3d() {}

Vector3d::Vector3d(double x, double y, double z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

void Vector3d::set(double x, double y, double z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

void Vector3d::nullify() {
	x = y = z = 0;
}

void Vector3d::add(const Vector3d & v) {
	x += v.x;
	y += v.y;
	z += v.z;
}

//

Vector3f operator*(const Vector3f& v, const float k) {
	Vector3f res;
	res.x = v.x*k;
	res.y += v.y*k;
	res.z += v.z*k;
	return res;
}

Vector3f operator+=(Vector3f& v1, const Vector3f& v2) {
	v1.add(v2);
	return v1;
}

Vector3f::Vector3f() {}

Vector3f::Vector3f(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

void Vector3f::set(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

void Vector3f::nullify() {
	x = y = z = 0;
}

void Vector3f::add(const Vector3f & v) {
	x += v.x;
	y += v.y;
	z += v.z;
}
