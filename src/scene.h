#ifndef SCENE_H
#define SCENE_H

#include "time.h"
#include "list.h"
#include "util.h"

typedef struct {
	TimeData timeData;
	Cuboid islandBoundingBox;
	List cannonBalls;
	List boats;
	float tesselationFactor;
	int score;
} Scene;

Scene scene_new();

#endif