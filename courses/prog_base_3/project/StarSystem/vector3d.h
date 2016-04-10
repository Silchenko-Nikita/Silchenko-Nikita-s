#pragma once

struct Vector3d {
	float x = 0, y = 0, z = 0;

	Vector3d();
	Vector3d(double x, double y, double z);
	void set(double x, double y, double z);
	void nullify();
	void add(const Vector3d & v);

	friend Vector3d& operator*(const Vector3d&, const float);
	friend Vector3d& operator+=(Vector3d&, const Vector3d&);
};