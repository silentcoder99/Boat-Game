#include "camera.h"
#include "util.h"

void pushCameraMatrix(Camera camera) {
	glPushMatrix();
	glTranslatef(0.0, 0.0, -camera.zoom);
	glRotatef(camera.rotation.x, 1.0, 0.0, 0.0);
	glRotatef(camera.rotation.y, 0.0, 1.0, 0.0);
}

void setPerspective(float aspectRatio) {
	float fov = 75.0;
	float nearPlane = 0.01;
	float farPlane = 100.0;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov, aspectRatio, nearPlane, farPlane);
	glMatrixMode(GL_MODELVIEW);
}

void setOrtho(float aspectRatio) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-aspectRatio, aspectRatio, -1.0, 1.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
}

void enableLighting() {
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}

void disableLighting() {
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
}