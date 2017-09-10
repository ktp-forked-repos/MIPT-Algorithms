#ifndef RAY_TRACING_GLUT_H
#define RAY_TRACING_GLUT_H

#define GLUT_BORDERLESS 0x0800

struct Glut {
	Glut(int width, int height, void (*display)(void)) {
		int argc = 0;
		char *argv[0];
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_BORDERLESS);

		int x = (glutGet(GLUT_SCREEN_WIDTH) - width) / 2;
		int y = (glutGet(GLUT_SCREEN_HEIGHT) - height) / 2;
		glutInitWindowSize(width, height);
		glutInitWindowPosition(x, y);

		glutCreateWindow("Ray tracing");
		init2D(width, height);
		glutDisplayFunc(display);
		glutMainLoop();
	}

	void init2D(int width, int height) {
		glClearColor(1, 1, 1, 0);
		glMatrixMode(GL_PROJECTION);
		gluOrtho2D(0, width, 0, height);
	}
};

#endif //RAY_TRACING_GLUT_H
