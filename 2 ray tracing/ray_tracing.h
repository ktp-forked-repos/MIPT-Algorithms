#ifndef RAY_TRACING_RAY_TRACING_H
#define RAY_TRACING_RAY_TRACING_H
#include <bits/stdc++.h>
using namespace std;

#include "matrix.h"
#include "objects.h"
#include "light.h"
#include "random.h"
#include "pretty_print.h"

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

	Intersect(bool is) : is(is), object(nullptr) {}

	Intersect(bool is, Object *object, const Point &point) : is(is), object(object), point(point) {}

	operator bool() const {
		return is;
	}
};

ostream &operator<<(ostream &out, Intersect intersect) {
	return printObject(out, "is", intersect.is, "object", intersect.object, "point", intersect.point);
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
//		getPixelColor(viewport.getPixel(h / 2, w / 2));
		for (int i = 0; i < h; ++i) {
			for (int j = 0; j < w; ++j) {
				matrix[i][j] = getPixelColor(viewport.getPixel(i, j));
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
//		for_each(triangles.begin(), triangles.end(), [this](Triangle &triangle) { objects.push_back(new Triangle(triangle)); });
	}

	void createLights() {
		referenceLight = {1, 200};
		lights = {{{w / 2, h / 2, 0}, 1.0f}};
	}

	Color getPixelColor(Point pixel) const {
		Intersect intersect = getIntersect({viewport.origin, pixel});
		if (!intersect) {
			return {0, 0, 0};
		}
		Object *object = intersect.object;
		Point point = intersect.point;
		Color color = object->material.color;

//		light
		LightPoint light = lights[0];
		Intersect intersect2 = getIntersect({light, point});
		if (intersect2.object == object && equals(point, intersect2.point)) {
			double distance = (pixel - light).squareLength();
			double ray2cos = object->getCos(point, light - point);
			assert(ray2cos > 0);
//			~ cos / r^2
			double kPower = light.power / referenceLight.power;
			double kDistance = (point - light).squareLength() / sqr(referenceLight.distance);
			double k = ray2cos * kPower / kDistance;
			if (k > 1) {
				k = 1;
			}
			assert(0 <= k && k <= 1);
			return color * k;
		}

		return {0, 0, 0};
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
		if (!p.first) {
			return {p.first};
		}
		return {p.first, object, ray.pointAt(p.second)};
	}
};

#endif //RAY_TRACING_RAY_TRACING_H
