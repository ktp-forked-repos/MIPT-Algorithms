#include <GL/glut.h>
#include <bits/stdc++.h>
using namespace std;

int w = 800;
int h = w;
#define all(container) container.begin(), container.end()
#include "debug.h"
#include "glut.h"
#include "ray_tracing.h"

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

int main() {
//	srand(time(nullptr));
	srand(0);
	RayTracing rt;
	matrix = rt.getMatrix();
	Glut glut(w, h, display);
	return 0;
}