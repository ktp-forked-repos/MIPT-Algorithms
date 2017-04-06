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

ostream &operator<<(ostream &out, Intersect intersect) {
	return printObject(out, "is", intersect.is, "object", intersect.object, "point", intersect.point);
}

class RayTracing;

istream &operator>>(istream &in, RayTracing &tracing);

struct RayTracing {
	vector<Sphere> spheres;
	vector<Triangle> triangles;

	Viewport viewport;
	vector<Object *> objects;
	ReferenceLight referenceLight;
	vector<LightPoint> lights;
	/*static */const Color BACKGROUND = black;
	static constexpr double BACKGROUND_LIGHT_RATIO = 0.2;

	void init() {
		createViewport();
		createObjects();
		createLights();
		postInit();
	}

	void postInit() {
		for_each(spheres.begin(), spheres.end(), [this](Sphere &sphere) { objects.push_back(new Sphere(sphere)); });
//		for_each(triangles.begin(), triangles.end(), [this](Triangle &triangle) { objects.push_back(new Triangle(triangle)); });
	}

	Matrix getMatrix() {
		assert(!objects.empty());
		Matrix matrix = createMatrix(h, w);
		for (int i = 0; i < h; ++i) {
			for (int j = 0; j < w; ++j) {
				matrix[i][j] = getPixelColor(viewport.getPixel(i, j));
			}
		}
		return matrix;
	}

	void createViewport() {
		viewport = {
				{w / 2, h / 2, 10000},
				{0,     h,     0},
				{0,     0,     0},
				{w,     h,     0},
				{w,     0,     0}
		};
	}

	void createObjects() {
		vector<Sphere> spheres = generateRandomSpheres();
		vector<Sphere> spheres2 = {
				{{0, h, -400}, 400},
//				{{w, 0, -400}, 400},
		};
		vector<Triangle> triangles = generateRandomTriangles();
	}

	void createLights() {
		referenceLight = {1, 200};
		lights = {{{w / 2, h / 2, 0}, 1.0f}};
	}

	Color getPixelColor(Point pixel) const {
		Intersect intersect = getIntersect({viewport.origin, pixel});
		if (!intersect) {
			return BACKGROUND;
		}
		static double x = 0;
		Object *object = intersect.object;
		Point point = intersect.point;
		Color color = object->material.color;

//		light
		double k = getLightRatio(pixel, intersect);
		return color * min(k + BACKGROUND_LIGHT_RATIO, 1.0);
	}

	double getLightRatio(Point pixel, Intersect intersect) const {
		Object *object = intersect.object;
		Point point = intersect.point;

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
			return k;
		}
		return 0;
	}

	Intersect getIntersect(const Ray &ray) const {
		auto comparator = [&ray](Object *o1, Object *o2) {
			Intersect i1 = o1->intersect(ray);
			Intersect i2 = o2->intersect(ray);
			if (i1.is && !i2.is) {
				return true;
			}
			if (!i1.is && i2.is) {
				return false;
			}
			return i1.is && i2.is && i1.t < i2.t;
		};
		Object *object = *min_element(objects.begin(), objects.end(), comparator);
		Intersect intersect = object->intersect(ray);
		assert(intersect.t >= 0);
		return intersect;
	}
};

#endif //RAY_TRACING_RAY_TRACING_H
