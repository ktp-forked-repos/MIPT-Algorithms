#ifndef RAY_TRACING_MATERIAL_H
#define RAY_TRACING_MATERIAL_H

#include <bits/stdc++.h>
#include "matrix.h"
using namespace std;

struct Material {
	string name;
	Color color;
	double alpha;
	double reflect;
	double refract;
};

Material cyanMaterial = {"cyan", {0, 1, 1}, 0, 0, 0};

#endif //RAY_TRACING_MATERIAL_H
