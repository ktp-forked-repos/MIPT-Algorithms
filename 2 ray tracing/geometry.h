#ifndef RAY_TRACING_GEOMETRY_H
#define RAY_TRACING_GEOMETRY_H
#include <bits/stdc++.h>
using namespace std;

const double eps = 1e-6;

struct Point {
	double x;
	double y;
	double z;

	Point() {}

	Point(int x, int y, int z) : x(x), y(y), z(z) {}

	Point(double x, double y, double z) : x(x), y(y), z(z) {}

	Point operator+(const Point &point) const {
		return {x + point.x, y + point.y, z + point.z};
	}

	Point operator-(const Point &point) const {
		return {x - point.x, y - point.y, z - point.z};
	}

	Point operator-() const {
		return {-x, -y, -z};
	}

	Point operator*(double f) const {
		return {x * f, y * f, z * f};
	}

	void operator*=(double f) {
		x *= f;
		y *= f;
		z *= f;
	}

	void operator+=(const Point &point) {
		x += point.x;
		y += point.y;
		z += point.z;
	}

	Point operator/(double f) const {
		return {x / f, y / f, z / f};
	}

	double operator^(const Point &p) const {
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

	Point normalized() const {
		return *this / length();
	}

	double length() const {
		return sqrt(x * x + y * y + z * z);
	}

	double squareLength() const {
		return x * x + y * y + z * z;
	}
};

bool equals(Point p, Point q, double epsToUse = eps) {
	return (p - q).length() < epsToUse;
}

double cos(Point p, Point q) {
	return (p ^ q) / p.length() / q.length();
}

double sqr(double x) {
	return x * x;
}

// Луч
// r = p + t * a
struct Ray {
//	точка на прямой
	Point p;
//	направляющий вектор
	Point a;

	Ray(const Point &p1, const Point &p2) : p(p1), a(p2 - p1) {}

	Point pointAt(double t) const {
		return p + a * t;
	}

	void moveForward(double d) {
		p += a * d / a.length();
	}
};

ostream &operator<<(ostream &out, Point point) {
//	return out << "(" << point.x << " " << point.y << " " << point.z << ")";
	return out << point.x << " " << point.y << " " << point.z;
}

ostream &operator<<(ostream &out, Ray ray) {
	return out << "(p=" << ray.p << ", a=" << ray.a << ")";
}

#endif //RAY_TRACING_GEOMETRY_H
