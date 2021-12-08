#ifndef WATER_H
#define WATER_H

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

#include "util.h"

typedef struct {
	float wavelength;
	float amplitude;
	float c;
	float t;
} Wave;

float waterFunction(Vec2 pos, float t);
Vec2 waterGradient(Vec2 pos, float t);

void drawWater(float time, int tesselations, float size);
void drawNormals(float time, int tesselations, float size);
void drawTangents(float time, int tesselations, float size);

#endif