#include "CImg.h"
using namespace cimg_library;
typedef CImg<unsigned char> Image;

#include <GL/glut.h>
#include <bits/stdc++.h>
using namespace std;

int h = 800;
int w = h;
int antiAliasingScale = 1;
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

void test() {
	Image src("image.jpg");
	int imageW = src.width();
	int imageH = src.height();
	matrix = createMatrix(h, w);
	for (int i = 0; i < h; ++i) {
		for (int j = 0; j < w; ++j) {
			matrix[i][j] = Color(white);
		}
	}
	for (int i = 0; i < imageH; ++i) {
		for (int j = 0; j < imageW; ++j) {
			matrix[h - i - 1][j] = getColor(src, i, j);
		}
	}
	Glut glut(w, h, display);
}

int main() {
//	srand(time(nullptr));
	srand(0);
//	ifstream in("examples2/transparent.rt");
	ifstream in("examples/quadrangles_texture.rt");
	RayTracing rt;
//	rt.init();
	in >> rt;

	rt.objects.push_back(new QuadrangleTexture(
			Point(700, 700, 200),
			Point(700, 300, 200),
			Point(100, 200, 400),
			Point(100, 700, 400),
			cyanMaterial,
			Image("image.jpg")
	));

	matrix = rt.getMatrix();
	Glut glut(w, h, display);
	return 0;
}