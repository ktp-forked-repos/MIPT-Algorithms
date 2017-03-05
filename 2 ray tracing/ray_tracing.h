#ifndef RAY_TRACING_RAY_TRACING_H
#define RAY_TRACING_RAY_TRACING_H
#include <bits/stdc++.h>
using namespace std;

#include "matrix.h"
#include "objects.h"
#include "light.h"

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

Point randomPoint() {
	return {rand() % w, rand() % h, -rand() % 200 - 100};
}

vector<Point> randomPoints(size_t n) {
	vector<Point> points;
	for (int i = 0; i < n; ++i) {
		points.push_back(randomPoint());
	}
	return points;
}

vector<Sphere> generateRandomSpheres(size_t n = 3) {
	vector<Sphere> spheres;
	for (int i = 0; i < n; ++i) {
		spheres.push_back({randomPoint(), rand() % 70 + 20, cyanMaterial});
	}
	return spheres;
}

vector<Triangle> generateRandomTriangles(size_t n = 3) {
	vector<Triangle> triangle;
	for (int i = 0; i < n; ++i) {
		triangle.push_back({randomPoints(3), cyanMaterial});
	}
	return triangle;
}

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

	void createObjects() const {
		vector<Sphere> spheres = generateRandomSpheres();
		vector<Triangle> triangles = generateRandomTriangles();
		for (Sphere &sphere : spheres) {
			objects.push_back(&sphere);
		}
		for (Triangle &triangle : triangles) {
			objects.push_back(&triangle);
		}
	}

	void createLights() {
		referenceLight = {1, 100};
		lights = {{w / 2, h / 2, 0, 1}};
	}

	Color getPixelColor(Point pixel) {
		Ray ray = {viewport.origin, pixel - viewport.origin};
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
		if (!p.first) {
			return {1, 1, 1};
		}
		return object->material.color;
	}
};

#endif //RAY_TRACING_RAY_TRACING_H
