#ifndef ISLAND_H
#define ISLAND_H

#include "util.h"
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
	float alpha;
	float theta;
	float firePower;
	int health;
	float cooldown;
} Island;

Color island_color;

Island island_new();
Cuboid island_getBoundingBox();
Vec3 island_getCannonEnd(Island island);
Vec3 island_getFireVelocity(Island island);
void drawIsland(Island island, GLuint texture, Scene* scene);

#endif