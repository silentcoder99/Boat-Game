#ifndef BOAT_H
#define BOAT_H

#include <stdbool.h>
#include "util.h"
#include "time.h" 

typedef struct {
	Vec3 pos;
	Vec3 rotation;
	float cannonAngle;
	int health;
	Circle boundingCircle;
	float coolDown;
} Boat;

Boat* boat_new();
void boat_update(void* data, void* scene);
void freeBoat(void* boat);
Vec3 boat_getFiringVelocity(Boat boat);
Vec3 boat_getCannonEnd(Boat boat);
bool boat_ballCollision(void* data, void* args);
void drawBoat(void* data, void* args);

#endif