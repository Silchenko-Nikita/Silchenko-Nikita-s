#pragma once

struct Vector3d {
	double x = 0, y = 0, z = 0;

	Vector3d();
	Vector3d(double x, double y, double z);
	void set(double x, double y, double z);
	void nullify();
	double length();
	Vector3d normalized();
	void add(const Vector3d & v);

	friend Vector3d operator*(const Vector3d&, const double);
	friend Vector3d operator+(const Vector3d& v1, const Vector3d& v2);
	friend Vector3d operator-(const Vector3d& v1, const Vector3d& v2);
	friend Vector3d operator+=(Vector3d&, const Vector3d&);

	double dot(Vector3d v);
	Vector3d cross(Vector3d v);

	static Vector3d crossed(Vector3d v1, Vector3d v2);
};

struct Vector3f {
	float x = 0, y = 0, z = 0;

	Vector3f();
	Vector3f(float x, float y, float z);
	void set(float x, float y, float z);
	void nullify();
	float length();
	Vector3f normalized();
	void add(const Vector3f & v);

	friend Vector3f operator*(const Vector3f&, const float);
	friend Vector3f operator+(const Vector3f& v1, const Vector3f& v2);
	friend Vector3f operator-(const Vector3f& v1, const Vector3f& v2);
	friend Vector3f operator+=(Vector3f&, const Vector3f&);

	float dot(Vector3f v);
	Vector3f cross(Vector3f v);

	static Vector3f crossed(Vector3f v1, Vector3f v2);
};