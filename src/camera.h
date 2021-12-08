#ifndef CAMERA_H
#define CAMERA_H

#if _WIN32
#   include <Windows.h>
#endif
#if __APPLE__
#   include <OpenGL/gl.h>
#   include <OpenGL/glu.h>
#   include <GLUT/glut.h>
#else
#   include <GL/gl.h>
#   include <GL/glu.h>
#   include <GL/glut.h>
#endif

#include <stdbool.h>
#include "util.h"

typedef struct {
	Vec3 rotation;
	float zoom;
} Camera;

typedef struct {
	bool leftButtonDown;
	bool rightButtonDown;
	Vec2 lastMousePos;
} Mouse;

void pushCameraMatrix(Camera camera);
void setPerspective(float aspectRatio);
void setOrtho(float aspectRatio);
void enableLighting();
void disableLighting();

#endif