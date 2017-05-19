#ifndef RAY_TRACING_OBJECTS_H
#define RAY_TRACING_OBJECTS_H
#include <bits/stdc++.h>
using namespace std;

#include "geometry.h"
#include "material.h"

class Object;

struct Intersect {
	bool is;
	Object *object;

	double t;
	Point point;

	Intersect(bool is) : is(is), object(nullptr) {}

	Intersect(bool is, Object *object, double t, const Point &point) : is(is), object(object), t(t), point(point) {}

	operator bool() const {
		return is;
	}
};

struct Object {
	Material material;

	Object() {}

	Object(const Material &material) : material(material) {}

	virtual Intersect intersect(Ray ray) = 0;

	virtual Point getNormal(Point point, Point positiveVector) const = 0;

	double getCos(Point point, Point vector) const {
		return cos(getNormal(point, vector), vector);
	};

//	virtual bool containsPoint(Point point) const = 0;

	Intersect makeIntersect(const Ray &ray, double t) {
		return {true, this, t, ray.pointAt(t)};
	}

	virtual bool isTexture() {
		return false;
	}

	virtual ~Object() {}
};

struct Sphere : public Object {
	Point center;
	double radius;

	Sphere() {}

	Sphere(const Point &center, double radius, const Material &material) : Object(material), center(center), radius(radius) {}

	Intersect intersect(Ray ray) override {
		double a = ray.a.squareLength();
		double b = (ray.p - center) ^ray.a;
		double c = (ray.p - center).squareLength() - radius * radius;
		double d = b * b - a * c;
		if (d < 0) {
			return {false};
		}

		double t1 = (-b - sqrt(d)) / a;
		double t2 = (-b + sqrt(d)) / a;
		if (t1 < 0) {
//			assert(t2 < 0);  // нет объектов внутри сферы
			return {false};
		}
//		dbg(ray.a, a, b, d, c, t1, t2);
		return makeIntersect(ray, t1);
	}

	Point getNormal(Point point, Point positiveVector) const override {
		return point - center;
	}

//	bool containsPoint(Point point) const override {
//		return abs((point - center).length() - radius) < eps;
//	}
};

bool isLeft(Point v0, Point v1, Point n) {
	return ((v0 * v1) ^ n) > 0;
}

struct Triangle : public Object {
	Point a;
	Point b;
	Point c;

	Triangle() {}

	Triangle(const Point &a, const Point &b, const Point &c, const Material &material) : Object(material), a(a), b(b), c(c) {}

	Triangle(const vector<Point> &points, const Material &material) : Triangle(points[0], points[1], points[2], material) {
		assert(points.size() == 3);
	}

	Intersect intersect(Ray ray) override {
		Point normal = (b - a) * (c - b);
		double d = -(normal ^ a);

		if (abs(normal ^ ray.a) < eps) {
			return {false};
		}

		double t = -(d + (normal ^ ray.p)) / (normal ^ ray.a);
		Point p = ray.p + ray.a * t;
		if (!isPointInside(p) || t < 0) {
			return {false};
		}
		return makeIntersect(ray, t);
	}

	bool isPointInside(Point p) const {
		Point normal = (b - a) * (c - b);
		return isLeft(b - a, p - a, normal)
		       && isLeft(c - b, p - b, normal)
		       && isLeft(a - c, p - c, normal);
	}

	Point getNormal(Point point, Point positiveVector) const override {
		Point normal = (b - a) * (c - b);
		if ((normal ^ positiveVector) < 0) {
			normal *= -1;
		}
		return normal;
	}
};

struct Quadrangle : public Object {
	Point a;
	Point b;
	Point c;
	Point d;

	Quadrangle() {}

	Quadrangle(const Point &a, const Point &b, const Point &c, const Point &d, const Material &material) : Object(material), a(a), b(b), c(c), d(d) {}

	Quadrangle(const vector<Point> &points, const Material &material) : Quadrangle(points[0], points[1], points[2], points[3], material) {
		assert(points.size() == 4);
	}

	Intersect intersect(Ray ray) override {
		Point normal = (b - a) * (c - b);
		double d = -(normal ^ a);

		if (abs(normal ^ ray.a) < eps) {
			return {false};
		}

		double t = -(d + (normal ^ ray.p)) / (normal ^ ray.a);
		Point p = ray.p + ray.a * t;
		if (!isPointInside(p) || t < 0) {
			return {false};
		}
		return makeIntersect(ray, t);
	}

	bool isPointInside(Point p) const {
		Point normal = (b - a) * (c - b);
		return isLeft(b - a, p - a, normal)
		       && isLeft(c - b, p - b, normal)
		       && isLeft(d - c, p - c, normal)
		       && isLeft(a - d, p - d, normal);
	}

	Point getNormal(Point point, Point positiveVector) const override {
		Point normal = (b - a) * (c - b);
		if ((normal ^ positiveVector) < 0) {
			normal *= -1;
		}
		return normal;
	}
};

Color getColor(Image image, int y, int x) {
	unsigned char r = image(x, y, 0, 0);
	unsigned char g = image(x, y, 0, 1);
	unsigned char b = image(x, y, 0, 2);
	return Color(r / 255.0, g / 255.0, b / 255.0);
}

struct QuadrangleTexture : public Quadrangle {
	Image image;

	QuadrangleTexture(const Point &a, const Point &b, const Point &c, const Point &d, const Material &material, Image image) : Quadrangle(a, b, c, d, material), image(image) {}

	Color getTextureColor(Point point) {
		Point ab = (b - a).normalized();
		Point ad = (d - a).normalized();
		Point abProjection = ab * ((point - a) ^ ab);
		Point adProjection = ad * ((point - a) ^ ad);
		int i = abProjection.length() / (b - a).length() * image.height();
		int j = adProjection.length() / (d - a).length() * image.width();
		return getColor(image, i, j);
	}

	virtual bool isTexture() {
		return true;
	}
};

#endif //RAY_TRACING_OBJECTS_H
