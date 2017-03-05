#ifndef RAY_TRACING_RAY_TRACING_H
#define RAY_TRACING_RAY_TRACING_H
#include <bits/stdc++.h>
using namespace std;

#include "matrix.h"
#include "objects.h"
#include "light.h"
#include "random.h"

struct Viewport {
	Point origin;
	Point topleft;
	Point bottomleft;
	Point topright;
	Point bottomright;

	Point getPixel(int i, int j) {
		Point dy = topleft - bottomleft;
		Point dx = bottomright - bottomleft;
		return bottomleft
		       + (dy * (i + .5) / h)
		       + (dx * (j + .5) / w);
	}
};

struct Intersect {
	bool is;
	Object *object;
	Point point;

	Intersect(bool is, Object *object, const Point &point) : is(is), object(object), point(point) {}

	operator bool() const {
		return is;
	}
};

struct RayTracing {
	Viewport viewport;
	vector<Object *> objects;
	ReferenceLight referenceLight;
	vector<LightPoint> lights;

	Matrix getMatrix() {
		Matrix matrix = createMatrix(h, w);
		createViewport();
		createObjects();
		createLights();
		for (int i = 0; i < h; ++i) {
			for (int j = 0; j < w; ++j) {
				Point pixel = viewport.getPixel(i, j);
				matrix[i][j] = getPixelColor(pixel);
			}
		}
		return matrix;
	}

	void createViewport() {
		viewport = {
				{w / 2, h / 2, 200},
				{0,     h,     0},
				{0,     0,     0},
				{w,     h,     0},
				{w,     0,     0}
		};
	}

	void createObjects() {
		vector<Sphere> spheres = generateRandomSpheres();
		vector<Triangle> triangles = generateRandomTriangles();
		for_each(spheres.begin(), spheres.end(), [this](Sphere &sphere) { objects.push_back(new Sphere(sphere)); });
		for_each(triangles.begin(), triangles.end(), [this](Triangle &triangle) { objects.push_back(new Triangle(triangle)); });
	}

	void createLights() {
		referenceLight = {1, 100};
		lights = {{w / 2, h / 2, 0, 1}};
	}

	Color getPixelColor(Point pixel) const {
		Intersect intersect = getIntersect({viewport.origin, pixel - viewport.origin});
		if (!intersect) {
			return {1, 1, 1};
		}
		return intersect.object->material.color;
	}

	Intersect getIntersect(const Ray &ray) const {
		auto comparator = [&ray](Object *o1, Object *o2) {
			auto p1 = o1->intersect(ray);
			auto p2 = o2->intersect(ray);
			if (p1.first && !p2.first) {
				return true;
			}
			if (!p1.first && p2.first) {
				return false;
			}
			return p1.first && p2.first && p1.second < p2.second;
		};
		Object *object = *min_element(objects.begin(), objects.end(), comparator);
		auto p = object->intersect(ray);
		assert(p.second >= 0);
		return {p.first, object, ray.pointAt(p.second)};
	}
};

#endif //RAY_TRACING_RAY_TRACING_H
