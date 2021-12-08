#ifndef COLLISION_H
#define COLLISION_H

#include "util.h"

bool circleCuboidCollision(Circle circle, Cuboid cuboid);
bool circleCircleCollision(Circle circleA, Circle circleB);
bool pointInsideCuboid(Vec3 point, Cuboid cuboid);

#endif