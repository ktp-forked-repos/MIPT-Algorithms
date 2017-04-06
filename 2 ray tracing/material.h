#ifndef RAY_TRACING_MATERIAL_H
#define RAY_TRACING_MATERIAL_H
#include <bits/stdc++.h>
using namespace std;

#include "matrix.h"

struct Material {
	string name;
	Color color;
	double alpha;
	double reflect;
	double refract;
};

Material cyanMaterial = {"cyan", {0, 1, 1}, 0, 0, 0};

ostream &operator<<(ostream &out, Material material) {
	return out << material.name;
}

#endif //RAY_TRACING_MATERIAL_H
