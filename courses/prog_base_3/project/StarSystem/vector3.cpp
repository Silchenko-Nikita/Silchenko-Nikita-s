#include "stdafx.h"

#include "vector3.h"

#include <math.h>

Vector3d operator*(const Vector3d& v, const double k) {
	Vector3d res;
	res.x = v.x*k;
	res.y = v.y*k;
	res.z = v.z*k;
	return res;
}

Vector3d operator+(const Vector3d& v1, const Vector3d& v2) {
	Vector3d res = v1;
	res.add(v2);
	return res;
}

Vector3d operator-(const Vector3d& v1, const Vector3d& v2) {
	Vector3d res = v1;
	res.x -= v2.x;
	res.y -= v2.y;
	res.z -= v2.z;
	return res;
}

Vector3d operator+=(Vector3d& v1, const Vector3d& v2) {
	v1.add(v2);
	return v1;
}

Vector3d::Vector3d() {
	x = y = z = 0.0;
}

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
	x = y = z = 0.0;
}

double Vector3d::length() {
	return sqrt(x*x + y*y + z*z);
}

Vector3d Vector3d::normalized() {
	double len = length();
	return Vector3d(x / len, y / len, z / len);
}

void Vector3d::add(const Vector3d & v) {
	x += v.x;
	y += v.y;
	z += v.z;
}

double Vector3d::dot(Vector3d v)
{
	return x * v.x + y * v.y + z * v.z;
}

Vector3d Vector3d::cross(Vector3d v)
{
	return Vector3d(x * v.x - v.y * z, z * v.x - v.z * x, x * v.y - v.x * y);
}

Vector3d Vector3d::crossed(Vector3d v1, Vector3d v2)
{
	return Vector3d(v1.y * v2.z - v2.y * v1.z, v1.z * v2.x - v2.z * v1.x, v1.x * v2.y - v2.x * v1.y);
}

//

Vector3f operator*(const Vector3f& v, const float k) {
	Vector3f res;
	res.x = v.x*k;
	res.y = v.y*k;
	res.z = v.z*k;
	return res;
}

Vector3f operator+(const Vector3f& v1, const Vector3f& v2) {
	Vector3f res = v1;
	res.add(v2);
	return res;
}

Vector3f operator-(const Vector3f& v1, const Vector3f& v2) {
	Vector3f res = v1;
	res.x -= v2.x;
	res.y -= v2.y;
	res.z -= v2.z;
	return res;
}

Vector3f operator+=(Vector3f& v1, const Vector3f& v2) {
	v1.add(v2);
	return v1;
}

Vector3f::Vector3f() {
	x = y = z = 0.0f;
}

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
	x = y = z = 0.0f;
}

float Vector3f::length() {
	return sqrtf(x*x + y*y + z*z);
}

Vector3f Vector3f::normalized() {
	float len = length();
	return Vector3f(x / len, y / len, z / len);
}

void Vector3f::add(const Vector3f & v) {
	x += v.x;
	y += v.y;
	z += v.z;
}

float Vector3f::dot(Vector3f v)
{
	return x * v.x + y * v.y + z * v.z;
}

Vector3f Vector3f::cross(Vector3f v)
{
	return Vector3f(x * v.x - v.y * z, z * v.x - v.z * x, x * v.y - v.x * y);
}


Vector3f Vector3f::crossed(Vector3f v1, Vector3f v2)
{
	return Vector3f(v1.y * v2.z - v2.y * v1.z, v1.z * v2.x - v2.z * v1.x, v1.x * v2.y - v2.x * v1.y);
}