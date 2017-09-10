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
	/*static *//*constexpr */double BACKGROUND_LIGHT_RATIO = 0.2;

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
				int i = q / wBig;
				int j = q % wBig;
				(*matrix)[i][j] = getPixelColor(viewport.getPixel(i, j));
			}
		};

		Matrix matrixScale = createMatrix(hBig, wBig);
		vector<thread> threads;
		int numberThreads = 4;
		assert(hBig * wBig % numberThreads == 0);
		int pixelsPerThread = hBig * wBig / numberThreads;
		for (int i = 0; i < numberThreads; ++i) {
			int qstart = pixelsPerThread * i;
			int qend = pixelsPerThread * (i + 1);
			threads.emplace_back(worker, &matrixScale, qstart, qend);
		}
		for (thread &t : threads) {
			t.join();
		}

		Matrix matrix = createMatrix(h, w);
		for (int i = 0; i < h; ++i) {
			for (int j = 0; j < w; ++j) {
				matrix[i][j] = Color(0, 0, 0);
				for (int iScale = 0; iScale < antiAliasingScale; ++iScale) {
					for (int jScale = 0; jScale < antiAliasingScale; ++jScale) {
						matrix[i][j] += matrixScale[i + iScale][j + iScale];
					}
				}
				matrix[i][j] /= antiAliasingScale * antiAliasingScale;
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

	static Color unionColors(Color color1, Color color2, double alpha2) {
		return color1 * (1 - alpha2) + color2 * alpha2;
	}

	Color getPixelColor(Ray ray) const {
		Intersect intersect = getIntersect(ray);
		if (!intersect) {
			return BACKGROUND_COLOR;
		}
		Object *object = intersect.object;
		Point point = intersect.point;
		Color color = object->material.color;
		if (object->isTexture()) {
			color = ((QuadrangleTexture *) object)->getTextureColor(intersect.point);
		}

//		light
		double alphaOwn = getLightRatio(intersect);
		alphaOwn = min(alphaOwn + BACKGROUND_LIGHT_RATIO, 1.0);

//		reflect
		double alphaReflect = object->material.reflect;
		Color reflectColor = alphaReflect == 0 ? BACKGROUND_COLOR : getReflectColor(ray, intersect);

//		refract
		double alphaRefract = object->material.alpha;
		double kRefract = object->material.refract;
		Color refractColor = kRefract == 0 ? BACKGROUND_COLOR : getRefractColor(ray, intersect, kRefract);

		color *= alphaOwn;
		color = unionColors(color, reflectColor, alphaReflect);
		color = unionColors(color, refractColor, 1 - alphaRefract);
		return color;
	}

	Color getReflectColor(Ray ray, Intersect intersect) const {
		Object *object = intersect.object;
		Point point = intersect.point;

		Point vector = -ray.a.normalized();
		Point normal = object->getNormal(point, vector).normalized();
		assert((vector ^ normal) > 0);
		Point projectionVectorOnNormal = normal * (vector ^ normal);
		Point ortogonalVectorOnNormal = vector - projectionVectorOnNormal;

		Point reflection = vector - ortogonalVectorOnNormal * 2;
		Ray ray2 = {point, point + reflection};
		ray2.moveForward(1e-3);
		return getPixelColor(ray2);
	}

	Color getRefractColor(Ray ray, Intersect intersect, double kRefract) const {
		Object *object = intersect.object;
		Point point = intersect.point;

		Point vector = ray.a.normalized();
		Point normal = object->getNormal(point, vector).normalized();
		if ((vector ^ normal) < 0) {
			normal *= -1;
			kRefract = 1 / kRefract;
		}
		assert((vector ^ normal) > 0);
		Point projectionVectorOnNormal = normal * (vector ^ normal);
		Point ortogonalVectorOnNormal = vector - projectionVectorOnNormal;

		Point refraction = projectionVectorOnNormal + ortogonalVectorOnNormal * kRefract;
		Ray ray2 = {point, point + refraction};
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
