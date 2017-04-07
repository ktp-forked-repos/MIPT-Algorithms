#ifndef RAY_TRACING_MATRIX_H_H
#define RAY_TRACING_MATRIX_H_H

#include <bits/stdc++.h>
using namespace std;

struct Color {
	double r;
	double g;
	double b;

	Color() {}

	Color(double r, double g, double b) : r(r), g(g), b(b) {}

	Color operator*(double k) const {
		return Color(r * k, g * k, b * k);
	}

	void operator*=(double k) {
		r *= k;
		g *= k;
		b *= k;
	}

	void operator/=(double k) {
		r /= k;
		g /= k;
		b /= k;
	}

	Color operator+(Color color) const {
		return Color(r + color.r, g + color.g, b + color.b);
	}

	void operator+=(Color color) {
		r += color.r;
		g += color.g;
		b += color.b;
	}
};

typedef vector<vector<Color>> Matrix;

Color white(1, 1, 1);
Color black(0, 0, 0);
Color cyan(0, 1, 1);

ostream &operator<<(ostream &out, Color color) {
	return out << "(" << color.r << " " << color.g << " " << color.b << ")";
}

Matrix createMatrix(int h, int w) {
	Matrix matrix(h);
	for (int i = 0; i < h; ++i) {
		matrix[i].resize(w);
	}
	return matrix;
}

#endif //RAY_TRACING_MATRIX_H_H
