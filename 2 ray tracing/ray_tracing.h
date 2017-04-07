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
	vector<Quadrangle> quadrangles;

	Viewport viewport;
	vector<Object *> objects;
	ReferenceLight referenceLight;
	vector<LightPoint> lights;
	/*static */const Color BACKGROUND_COLOR = black;
	/*static *//*constexpr */double BACKGROUND_LIGHT_RATIO = 0.1;

	void init() {
		createViewport();
		createObjects();
		createLights();
		postInit();
	}

	void postInit() {
		for_each(spheres.begin(), spheres.end(), [this](Sphere &sphere) { objects.push_back(new Sphere(sphere)); });
		for_each(triangles.begin(), triangles.end(), [this](Triangle &triangle) { objects.push_back(new Triangle(triangle)); });
		for_each(quadrangles.begin(), quadrangles.end(), [this](Quadrangle &quadrangle) { objects.push_back(new Quadrangle(quadrangle)); });
	}

	Matrix getMatrix() {
		assert(!objects.empty());

		auto worker = [this](Matrix *matrix, int qstart, int qend) {
			for (int q = qstart; q < qend; ++q) {
				int i = q / w;
				int j = q % w;
				(*matrix)[i][j] = getPixelColor(viewport.getPixel(i, j));
			}
		};

		Matrix matrix = createMatrix(h, w);
		vector<thread> threads;
		int numberThreads = 4;
		assert(h * w % numberThreads == 0);
		int pixelsPerThread = h * w / numberThreads;
		for (int i = 0; i < numberThreads; ++i) {
			int qstart = pixelsPerThread * i;
			int qend = pixelsPerThread * (i + 1);
			threads.emplace_back(worker, &matrix, qstart, qend);
		}
		for (thread &t : threads) {
			t.join();
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
//		spheres = generateRandomSpheres(5);
//		triangles = generateRandomTriangles(5);
		quadrangles = generateRandomQuadrangles();
	}

	void createLights() {
		referenceLight = {1, 200};
		lights = {{{w / 2, h / 2, 0}, 1.0f}};
	}

	Color getPixelColor(Point pixel) const {
		return getPixelColor({viewport.origin, pixel});
	}

	Color getPixelColor(Ray ray) const {
		Intersect intersect = getIntersect(ray);
		if (!intersect) {
			return BACKGROUND_COLOR;
		}
		Object *object = intersect.object;
		Point point = intersect.point;
		Color color = object->material.color;

//		light
		double kOwn = getLightRatio(intersect);
		kOwn = min(kOwn + BACKGROUND_LIGHT_RATIO, 1.0);

//		reflect
		double kReflect = object->material.reflect;
		Color reflectColor = kReflect == 0 ? BACKGROUND_COLOR : getReflectColor(ray, intersect);

		return color * kOwn * (1 - kReflect) + reflectColor * kReflect;
	}

	Color getReflectColor(Ray ray, Intersect intersect) const {
		Object *object = intersect.object;
		Point point = intersect.point;

		Point vector = -ray.a.normalized();
		Point normal = object->getNormal(point, vector).normalized();
		Point projectionVectorOnNormal = normal * (vector ^ normal);
		assert((vector ^ normal) > 0);
		Point ortogonalVectorOnNormal = vector - projectionVectorOnNormal;
		Point reflection = vector - ortogonalVectorOnNormal * 2;

		Ray ray2 = {point, point + reflection};
		ray2.moveForward(1e-3);
		return getPixelColor(ray2);
	}

	double getLightRatio(Intersect intersect) const {
		Object *object = intersect.object;
		Point point = intersect.point;

		double k = 0;
		for (LightPoint light : lights) {
			Intersect intersect2 = getIntersect({light, point});
			if (intersect2.object == object && equals(point, intersect2.point)) {
				double ray2cos = object->getCos(point, light - point);
				assert(ray2cos > 0);
//			    ~ cos / r^2
				double kPower = light.power / referenceLight.power;
				double kDistance = (point - light).squareLength() / sqr(referenceLight.distance);
				double kAddidtion = ray2cos * kPower / kDistance;
				assert(0 <= kAddidtion);
				k += kAddidtion;
			}
		}
		return k;
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
		assert(!intersect.is || intersect.t >= 0);
		return intersect;
	}
};

#endif //RAY_TRACING_RAY_TRACING_H
