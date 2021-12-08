#ifndef CONFIG_H
#define CONFIG_H

#include <stdbool.h>

#define GRAVITY 1.0

#define BALL_RADIUS 0.015
#define BALL_COLLISION_TIME 0.01
#define SPREAD_RATE	0.1
#define DEFENCE_LIFETIME 1.75

#define BOAT_MAX_HEALTH 1
#define BOAT_RADIUS 0.08
#define BOAT_CANNON_LENGTH 0.05
#define BOAT_SPEED 0.05
#define SPAWN_DISTANCE 2.0
#define SPAWN_TIME 10.0

#define ISLAND_MAX_HEALTH 100.0
#define FIRE_VELOCITY 1.2
#define ISLAND_MAX_VELOCITY 2.0
#define COOLDOWN_TIME 1.0

#define WATER_SIZE 10.0

#define WATER_SEGMENTS 64
#define SPHERE_SEGMENTS 16
#define PATH_SEGMENTS 32

#define PATH_STEPS 1000

#define WATER_COLOR (Vec4) { 0.0, 0.6, 1.0, 0.75 }
#define ISLAND_COLOR (Vec4) { 0.41, 0.41, 0.41, 1.0 }
#define BOAT_COLOR (Vec4) { 1.0, 0.0, 0.0, 1.0 }
#define BALL_COLOR (Vec4) { 0.5, 0.5, 0.5, 1.0 }

typedef struct {
	bool wireframe;
	bool normals;
	bool tangents;
	bool lighting;
	bool animateWater;
	bool gameOver;
	float aspectRatio;
} Settings;

Settings settings_new();

#endif