#ifndef RAY_TRACING_RT_H
#define RAY_TRACING_RT_H
#include <bits/stdc++.h>
using namespace std;

#include "ray_tracing.h"

void trim(string &s) {
	s.erase(0, s.find_first_not_of(" \t"));
	s.erase(s.find_last_not_of(" \t") + 1);
}

istream &operator>>(istream &in, Point &point) {
	return in >> point.x >> point.y >> point.z;
}

map<string, Material> materials;

istream &operator>>(istream &in, Color &color) {
	return in >> color.r >> color.g >> color.b;
}

template<typename T>
T read(istream &in, string expectedName) {
	string name;
	T value;
	in >> name >> value;
	assert(name == expectedName);
	return value;
}

Point readPoint(istream &in, string expectedName) {
	return read<Point>(in, expectedName);
}

double readDouble(istream &in, string expectedName) {
	return read<double>(in, expectedName);
}

string readString(istream &in, string expectedName) {
	return read<string>(in, expectedName);
}

Color readColor(istream &in, string expectedName) {
	return read<Color>(in, expectedName);
}

Material readMaterial(istream &in, string expectedName) {
	string id = readString(in, expectedName);
	assert(materials.find(id) != materials.end());
	return materials[id];
}

istream &operator>>(istream &in, Viewport &viewport) {
	viewport.origin = readPoint(in, "origin");
	viewport.topleft = readPoint(in, "topleft");
	viewport.bottomleft = readPoint(in, "bottomleft");
	viewport.topright = readPoint(in, "topright");
	viewport.bottomright = viewport.bottomleft + (viewport.topright - viewport.topleft);
	return in;
}

istream &operator>>(istream &in, Material &material) {
	material.name = readString(in, "name");
	material.color = readColor(in, "color");
	material.alpha = readDouble(in, "alpha");
	material.reflect = readDouble(in, "reflect");
	material.refract = readDouble(in, "refract");
	assert(0 <= material.alpha && material.alpha <= 1);
	assert(0 <= material.reflect && material.reflect <= 1);
	assert(0 <= material.refract);
	return in;
}

istream &operator>>(istream &in, ReferenceLight &referenceLight) {
	referenceLight.power = readDouble(in, "power");
	referenceLight.distance = readDouble(in, "distance");
	return in;
}

istream &operator>>(istream &in, LightPoint &light) {
	(Point &) light = readPoint(in, "coords");
	light.power = readDouble(in, "power");
	return in;
}

istream &operator>>(istream &in, Sphere &sphere) {
	sphere.center = readPoint(in, "coords");
	sphere.radius = readDouble(in, "radius");
	sphere.material = readMaterial(in, "material");
	return in;
}

istream &operator>>(istream &in, Triangle &triangle) {
	triangle.a = readPoint(in, "vertex");
	triangle.b = readPoint(in, "vertex");
	triangle.c = readPoint(in, "vertex");
	triangle.material = readMaterial(in, "material");
	return in;
}

istream &operator>>(istream &in, Quadrangle &quadrangle) {
	quadrangle.a = readPoint(in, "vertex");
	quadrangle.b = readPoint(in, "vertex");
	quadrangle.c = readPoint(in, "vertex");
	quadrangle.d = readPoint(in, "vertex");
	quadrangle.material = readMaterial(in, "material");
	return in;
}

istream &operator>>(istream &in, RayTracing &tracing) {
	stringstream ss;
	{
		string line;
		while (getline(in, line)) {
			if (line.find('#') != string::npos) {
				line.erase(line.find('#'));
			}
			trim(line);
			if (line.empty()) {
				continue;
			}
			ss << line << "\n";
		}
	}

	string token;
	while (ss >> token) {
		if (token == "viewport") {
			ss >> tracing.viewport;
			ss >> token;
			assert(token == "endviewport");
		} else if (token == "materials") {
			while (ss >> token && token != "endmaterials") {
				assert(token == "entry");
				Material material;
				ss >> material;
				materials[material.name] = material;
				ss >> token;
				assert(token == "endentry");
			}
		} else if (token == "lights") {
			while (ss >> token && token != "endlights") {
				if (token == "reference") {
					ss >> tracing.referenceLight;
					ss >> token;
					assert(token == "endreference");
				} else if (token == "point") {
					LightPoint light;
					ss >> light;
					tracing.lights.push_back(light);
					ss >> token;
					assert(token == "endpoint");
				}
			}
		} else if (token == "geometry") {
			while (ss >> token && token != "endgeometry") {
				if (token == "sphere") {
					Sphere sphere;
					ss >> sphere;
					tracing.spheres.push_back(sphere);
					ss >> token;
					assert(token == "endsphere");
				} else if (token == "triangle") {
					Triangle triangle;
					ss >> triangle;
					tracing.triangles.push_back(triangle);
					ss >> token;
					assert(token == "endtriangle");
				} else if (token == "quadrangle") {
					Quadrangle quadrangle;
					ss >> quadrangle;
					tracing.quadrangles.push_back(quadrangle);
					ss >> token;
					assert(token == "endquadrangle");
				}
			}
		}
	}
	tracing.postInit();
	return in;
}

#endif //RAY_TRACING_RT_H
