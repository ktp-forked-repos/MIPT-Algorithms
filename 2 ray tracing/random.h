#ifndef RAY_TRACING_RANDOM_H
#define RAY_TRACING_RANDOM_H
#include <bits/stdc++.h>
using namespace std;

#include "geometry.h"
#include "objects.h"

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

#endif //RAY_TRACING_RANDOM_H
