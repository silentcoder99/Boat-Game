#include <stdio.h>
#include <stdbool.h>
#include "time.h"
#include "draw.h"

float frameRateInterval = 1.0;

void updateTime(TimeData* timeObj, bool animate) {
	timeObj->elapsedTime = glutGet(GLUT_ELAPSED_TIME);
	timeObj->elapsedTime /= 1000.0;

	timeObj->dt = timeObj->elapsedTime - timeObj->lastT;
	timeObj->lastT = timeObj->elapsedTime;

	timeObj->frameCount++;

	if (animate) {
		timeObj->t += timeObj->dt;
	}

	float timeSinceFrameRate = timeObj->elapsedTime - timeObj->lastFrameRateTime;
	if (timeSinceFrameRate >= frameRateInterval) {
		timeObj->frameRate = timeObj->frameCount / timeSinceFrameRate;
		timeObj->frameTime = 1.0 / timeObj->frameRate * 1000.0;

		timeObj->lastFrameRateTime = timeObj->elapsedTime;
		timeObj->frameCount = 0;
	}
}

void drawFrameInfo(TimeData timeObj) {
	char str[50];
	snprintf(str, 50, "fr (f/s): %7.2f\nft (ms/f):%7.2f",
		timeObj.frameRate, timeObj.frameTime);

	drawText((Vec2) { 0.4, 0.92 }, (Color) { 1.0, 1.0, 1.0 }, str);
}