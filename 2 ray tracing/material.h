#ifndef RAY_TRACING_MATERIAL_H
#define RAY_TRACING_MATERIAL_H

#include <bits/stdc++.h>
#include "matrix.h"
using namespace std;

struct Material {
	string name;
	Color color;
	float alpha;
	float reflect;
	float refract;
};

Material cyanMaterial = {"cyan", {0, 1, 1}, 0, 0, 0};

#endif //RAY_TRACING_MATERIAL_H
