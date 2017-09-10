#ifndef RAY_TRACING_RANDOM_H
#define RAY_TRACING_RANDOM_H
#include <bits/stdc++.h>
using namespace std;

#include "geometry.h"
#include "objects.h"

// [a, b]
double randf(double a, double b) {
	assert(a <= b);
	static random_device rd;
	static default_random_engine engine(rd());
	uniform_real_distribution<double> distribution(a, b);
	return distribution(engine);
}

Point randomPoint() {
	return {randf(0, w), randf(0, h), randf(100, 300)};
}

vector<Point> randomPoints(size_t n) {
	vector<Point> points;
	for (int i = 0; i < n; ++i) {
		points.push_back(randomPoint());
	}
	return points;
}

vector<Sphere> generateRandomSpheres(size_t n = 10) {
	vector<Sphere> spheres;
	for (int i = 0; i < n; ++i) {
		spheres.push_back({randomPoint(), randf(20, 60), cyanMaterial});
	}
	return spheres;
}

vector<Triangle> generateRandomTriangles(size_t n = 10) {
	vector<Triangle> triangle;
	for (int i = 0; i < n; ++i) {
		triangle.push_back({randomPoints(3), cyanMaterial});
	}
	return triangle;
}

vector<Quadrangle> generateRandomQuadrangles(size_t n = 10) {
	vector<Quadrangle> quadrangles;
	for (int i = 0; i < n; ++i) {
		quadrangles.push_back({randomPoints(4), cyanMaterial});
	}
	return quadrangles;
}

#endif //RAY_TRACING_RANDOM_H
