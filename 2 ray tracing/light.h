#ifndef RAY_TRACING_LIGHT_H
#define RAY_TRACING_LIGHT_H
#include <bits/stdc++.h>
using namespace std;

#include "geometry.h"

struct ReferenceLight {
	double power;
	double distance;

	ReferenceLight() {}

	ReferenceLight(double power, double distance) : power(power), distance(distance) {}
};

struct LightPoint : public Point {
	double power;

	LightPoint(Point point, double power) : Point(point), power(power) {}
};

#endif //RAY_TRACING_LIGHT_H
