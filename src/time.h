#ifndef TIME_H
#define TIME_H

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

typedef struct {
	float t;
	float lastT;
	float dt;
	float elapsedTime;
	float frameRate;
	float frameTime;
	int frameCount;
	float lastFrameRateTime;
} TimeData;

void updateTime(TimeData* timeObj, bool animate);
void drawFrameInfo(TimeData timeObj);

#endif