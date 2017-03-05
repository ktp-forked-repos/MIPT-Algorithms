#ifndef RAY_TRACING_GEOMETRY_H
#define RAY_TRACING_GEOMETRY_H
#include <bits/stdc++.h>
using namespace std;

const float eps = 1e-6;

struct Point {
	float x;
	float y;
	float z;

	Point() {}

	Point(float x, float y, float z) : x(x), y(y), z(z) {}

	Point operator+(const Point &point) const {
		return {x + point.x, y + point.y, z + point.z};
	}

	Point operator-(const Point &point) const {
		return {x - point.x, y - point.y, z - point.z};
	}

	Point operator*(float f) const {
		return {x * f, y * f, z * f};
	}

	Point operator/(float f) const {
		return {x / f, y / f, z / f};
	}

	float operator^(const Point &p) const {
		return x * p.x + y * p.y + z * p.z;
	}

	Point operator*(const Point &p) const {
//		|  i   j   k  |
//		|  x   y   z  |
//		| p.x p.y p.z |
		return {y * p.z - z * p.y,
		        -(x * p.z - z * p.x),
		        x * p.y - y * p.x};
	}

	float length() const {
		return sqrt(x * x + y * y + z * z);
	}

	float squareLength() const {
		return x * x + y * y + z * z;
	}
};

// r = p + t * a
struct Ray {
//	точка на прямой
	Point p;
//	направляющий вектор
	Point a;

	Ray(const Point &p, const Point &a) : p(p), a(a) {}

	Point pointAt(float t) const {
		return p + a * t;
	}
};

ostream &operator<<(ostream &out, Point point) {
	return out << "(" << point.x << " " << point.y << " " << point.z << ")";
}

ostream &operator<<(ostream &out, Ray ray) {
	return out << "(p=" << ray.p << ", a=" << ray.a << ")";
}

#endif //RAY_TRACING_GEOMETRY_H
