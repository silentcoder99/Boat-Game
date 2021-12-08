#include <stdbool.h>
#include <math.h>
#include "util.h"

static float squaredDistBetween(Vec3 pointA, Vec3 pointB) {
	return pow(pointA.x - pointB.x, 2.0) +
		   pow(pointA.y - pointB.y, 2.0) +
		   pow(pointA.z - pointB.z, 2.0);
}

static Vec3 closestPointOnCuboid(Vec3 point, Cuboid cuboid) {
	return (Vec3) {
		clamp(point.x, cuboid.min.x, cuboid.max.x),
		clamp(point.y, cuboid.min.y, cuboid.max.y),
		clamp(point.z, cuboid.min.z, cuboid.max.z)
	};
}

static float squaredDistToCuboid(Vec3 point, Cuboid cuboid) {
	return squaredDistBetween(point, closestPointOnCuboid(point, cuboid));
}

bool circleCuboidCollision(Circle circle, Cuboid cuboid) {
	float squaredRadius = pow(circle.radius, 2);
	return squaredRadius >= squaredDistToCuboid(circle.centre, cuboid);
}

bool circleCircleCollision(Circle circleA, Circle circleB) {
	float squaredRadii = pow(circleA.radius, 2.0) + pow(circleB.radius, 2.0);
	return squaredRadii >= squaredDistBetween(circleA.centre, circleB.centre);
}

bool pointInsideCuboid(Vec3 point, Cuboid cuboid) {
	Vec3 min = cuboid.min;
	Vec3 max = cuboid.max;

	return point.x >= min.x && point.x <= max.x &&
		   point.y >= min.y && point.y <= max.y &&
		   point.z >= min.z && point.z <= max.z;
}