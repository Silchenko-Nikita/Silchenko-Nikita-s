#pragma once

struct Vector3d {
	double x = 0, y = 0, z = 0;

	Vector3d();
	Vector3d(double x, double y, double z);
	void set(double x, double y, double z);
	void nullify();
	void add(const Vector3d & v);

	friend Vector3d operator*(const Vector3d&, const double);
	friend Vector3d operator+=(Vector3d&, const Vector3d&);
};

struct Vector3f {
	float x = 0, y = 0, z = 0;

	Vector3f();
	Vector3f(float x, float y, float z);
	void set(float x, float y, float z);
	void nullify();
	void add(const Vector3f & v);

	friend Vector3f operator*(const Vector3f&, const float);
	friend Vector3f operator+=(Vector3f&, const Vector3f&);
};