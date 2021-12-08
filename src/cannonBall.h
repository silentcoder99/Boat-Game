#ifndef CANNONBALL_H
#define CANNONBALL_H

#include <stdlib.h>
#include <stdbool.h>
#include "util.h"
#include "list.h"
#include "scene.h"

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

typedef struct {
	Vec3 pos;
	Vec3 vel;
	float collisionTimer; // Time until cannon ball can collide with objects
	bool isDefence;
	float lifeTime;
	Circle boundingCircle;
	List* list;
} CannonBall;

CannonBall* ball_new(Vec3 pos, Vec3 vel, bool isDefence);
void ball_hitObject(CannonBall* ball, int* targetHealth);
void freeCannonBall(void* cannonBall);
void drawPath(Vec3 pos, Vec3 vel, Scene* scene);
void ball_drawPath(void* data, void* arg);
void drawBall(void* data, void* arg);

#endif