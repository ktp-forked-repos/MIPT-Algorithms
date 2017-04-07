#include <GL/glut.h>
#include <bits/stdc++.h>
using namespace std;

int h = 800;
int w = h;
int antiAliasingScale = 4;
int hBig = h * antiAliasingScale;
int wBig = w * antiAliasingScale;
#include "debug.h"
#include "glut.h"
#include "ray_tracing.h"
#include "rt.h"

Matrix matrix;

void glPoint(int x, int y, double r, double g, double b) {
	glColor3f(r, g, b);
	glVertex2i(x + 1, y);
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT);

	glBegin(GL_POINTS);
	for (int i = 0; i < h; ++i) {
		for (int j = 0; j < w; ++j) {
			Color color = matrix[i][j];
			glPoint(j, i, color.r, color.g, color.b);
		}
	}
	glEnd();

	glFlush();
}

void printSpheres() {
	vector<Sphere> spheres = generateRandomSpheres();
	for (Sphere sphere : spheres) {
		cout << "sphere" << endl;
		cout << "    coords " << sphere.center << endl;
		cout << "    radius " << sphere.radius << endl;
		cout << "    material " << sphere.material << endl;
		cout << "endsphere" << endl;
	}
}

void printTriangles() {
	vector<Triangle> triangles = generateRandomTriangles();
	for (Triangle triangle : triangles) {
		cout << "triangle" << endl;
		cout << "    vertex " << triangle.a << endl;
		cout << "    vertex " << triangle.b << endl;
		cout << "    vertex " << triangle.c << endl;
		cout << "    material " << triangle.material << endl;
		cout << "endtriangle" << endl;
	}
}

int main() {
//	srand(time(nullptr));
	srand(0);
	ifstream in("rt/reflection.rt");
	RayTracing rt;
//	rt.init();
	in >> rt;
	matrix = rt.getMatrix();
	Glut glut(w, h, display);
	return 0;
}