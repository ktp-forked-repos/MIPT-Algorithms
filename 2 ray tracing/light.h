#ifndef RAY_TRACING_LIGHT_H
#define RAY_TRACING_LIGHT_H
#include <bits/stdc++.h>
using namespace std;

#include "geometry.h"

struct ReferenceLight {
	float power;
	float distance;

	ReferenceLight() {}

	ReferenceLight(float power, float distance) : power(power), distance(distance) {}
};

struct LightPoint : public Point {
	float power;

	LightPoint(float x, float y, float z, float power) : Point(x, y, z), power(power) {}

	LightPoint(Point point, float power) : Point(point), power(power) {}
};

#endif //RAY_TRACING_LIGHT_H
