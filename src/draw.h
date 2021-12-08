#ifndef DRAW_H
#define DRAW_H

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

#include <stdint.h>
#include "util.h"

void drawAxes(float len);
void drawTangent(float m);
void drawNormal(float m);
void drawTrapezoid(float a, float b, float h, Vec2 pos);
void drawRect(Rect rect, Color color);
void drawRectOutline(Rect rect, Color color, float width);
void drawQuad(Vec3 pos, Vec2 size);
void drawCuboid(Cuboid cuboid, GLuint* texture);
void drawHealthBar(Vec2 pos, float value, Color color);
void drawCircle(Circle circle);
void drawSphere(Circle circle, float tesselationFactor);
void drawHealthBar(Vec2 pos, float value, Color color);
void drawText(Vec2 pos, Color color, char* str);

void drawVec(Vec3 pos, Vec3 magnitude, float scale, Color color);

void setMaterial(Vec4 color, float specular, float shininess);
uint32_t loadTexture(const char* filename);

#endif