#ifndef RAY_TRACING_OBJECTS_H
#define RAY_TRACING_OBJECTS_H
#include <bits/stdc++.h>
using namespace std;

#include "geometry.h"
#include "material.h"

struct Object {
	Material material;

	Object(const Material &material) : material(material) {}

	virtual pair<bool, double> intersect(Ray ray) const = 0;

	virtual double getCos(Point point, Point v) const = 0;

	virtual bool containsPoint(Point point) const = 0;

	virtual ~Object() {}
};

struct Sphere : public Object {
	Point center;
	double radius;

	Sphere(const Point &center, double radius, const Material &material = cyanMaterial) : Object(material), center(center), radius(radius) {}

	pair<bool, double> intersect(Ray ray) const override {
		double a = ray.a.squareLength();
		double b = (ray.p - center) ^ray.a;
		double c = (ray.p - center).squareLength() - radius * radius;
		double d = b * b - a * c;
		if (d < 0) {
			return {false, 0};
		}

		double t = (-b - sqrt(d)) / a;
		if (t < 0) {
			return {false, 0};
		}
		return {true, t};
	}

	double getCos(Point point, Point vector) const override {
		return cos(point - center, vector);
	}

	bool containsPoint(Point point) const override {
		return abs((point - center).length() - radius) < eps;
	}
};

struct Triangle : public Object {
	Point a;
	Point b;
	Point c;

	Triangle(const Point &a, const Point &b, const Point &c, const Material &material = cyanMaterial) : Object(material), a(a), b(b), c(c) {}

	Triangle(const vector<Point> &points, const Material &material = cyanMaterial) : Triangle(points[0], points[1], points[2], material) {
		assert(points.size() == 3);
	}

	pair<bool, double> intersect(Ray ray) const override {
		Point ab = b - a;
		Point ac = c - a;
		Point n = ab * ac;
		double d = -(n ^ a);

		if (abs(n ^ ray.a) < eps) {
			return {false, 0};
		}

		double t = -(d + (n ^ ray.p)) / (n ^ ray.a);
		Point p = ray.p + ray.a * t;
		if (!isPointInside(p) || t < 0) {
			return {false, 0};
		}
		return {true, t};
	}

	bool isPointInside(Point p) const {
		Point ap = p - a;
		Point ab = b - a;
		Point bc = c - b;
		Point n = ab * bc;
		return isLeft(b - a, p - a, n)
		       && isLeft(c - b, p - b, n)
		       && isLeft(a - c, p - c, n);
	}

	static bool isLeft(Point v0, Point v1, Point n) {
		return ((v0 * v1) ^ n) > 0;
	}
};

#endif //RAY_TRACING_OBJECTS_H
