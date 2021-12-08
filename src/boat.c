#include <stdlib.h>
#include <math.h>
#include "boat.h"
#include "config.h"
#include "water.h"
#include "draw.h"
#include "time.h"
#include "collision.h"
#include "cannonBall.h"
#include "camera.h"
#include "island.h"
#include "scene.h"

static Vec3 cannonOffset = { 0.04, 0.022, 0.0 };

Vec2 getSpawnPos() {
	float angle = 2.0 * M_PI * (double) rand() / RAND_MAX;
	printf("Angle: %f\n", angle);

	Vec2 spawnPos = vec2_polar2vec(angle, SPAWN_DISTANCE);
	printf("Position: %f, %f\n", spawnPos.x, spawnPos.y);

	return spawnPos;
}

Boat* boat_new() {
	Boat* boat = malloc(sizeof(Boat));
	if (boat == NULL) {
		printf("error: failed to alloc boat memory\n");
		return NULL;
	}

	Vec2 spawnPos = getSpawnPos();
	Vec3 initialPos = { spawnPos.x, waterFunction(spawnPos, 0.0), spawnPos.y };
	
	boat->pos = initialPos;
	boat->rotation = (Vec3){ 0.0, 0.0, 0.0 };
	boat->cannonAngle = deg2rad(30.0);
	boat->health = BOAT_MAX_HEALTH;
	boat->boundingCircle = (Circle){
		initialPos,
		BOAT_RADIUS
	};
	boat->coolDown = 0.0;

	Vec3 toIsland = vec3_multiply(boat->pos, -1.0);
	boat->rotation.y = vec2_arg((Vec2) { toIsland.x, toIsland.z }) - M_PI / 2.0;

	return boat;
}

void boat_fireCannon(Boat* boat, List* cannonBallList) {
	Vec3 pos = boat_getCannonEnd(*boat);
	Vec3 vel = boat_getFiringVelocity(*boat);
	CannonBall* ball = ball_new(pos, vel, false);
	list_pushFront(cannonBallList, ball);
	ball->list = cannonBallList;
	boat->coolDown = COOLDOWN_TIME;
}

void boat_update(void* data, void* arg) {
	Boat* boat = data;
	Scene* scene = arg;

	if (boat->health <= 0) {
		list_remove(&scene->boats, boat);
		scene->score++;
		return;
	}

	Vec2 gridPos = { boat->pos.x, boat->pos.z };

	Vec3 toIsland = vec3_multiply(boat->pos, -1.0);
	toIsland.y = 0;
	toIsland = vec3_normalise(toIsland);
	Vec3 velocity = vec3_multiply(toIsland, BOAT_SPEED * scene->timeData.dt);

	boat->pos.y = waterFunction(gridPos, scene->timeData.t);
	boat->pos = vec3_add(boat->pos, velocity);

	boat->rotation.x = -atan(waterGradient(gridPos, scene->timeData.t).y);
	boat->rotation.z = atan(waterGradient(gridPos, scene->timeData.t).x);

	boat->boundingCircle.centre = boat->pos;
	boat->coolDown -= scene->timeData.dt;
	boat->cannonAngle = clamp(boat->cannonAngle, 0.0, M_PI / 2.0);

	//Boat firing
	if (boat->coolDown <= 0) {
		boat_fireCannon(boat, &scene->cannonBalls);
	}

	//Boat/island collision
	if (circleCuboidCollision(boat->boundingCircle, island_getBoundingBox())) {
		list_remove(&scene->boats, boat);
	}
}

void freeBoat(void* boat) {
	free(boat);
}

Vec3 boat_getFiringVelocity(Boat boat) {
	return vec3_polar2vec(boat.cannonAngle, boat.rotation.y, FIRE_VELOCITY);
}

Vec3 boat_getCannonEnd(Boat boat) {
	Vec3 cannonVector = vec3_polar2vec(boat.cannonAngle, 0, BOAT_CANNON_LENGTH);
	Vec3 cannonEndOffset = vec3_add(cannonVector, cannonOffset);

	cannonEndOffset = vec3_yaw(cannonEndOffset, -boat.rotation.y);
	
	//cannonEndOffset = vec2_rotate(cannonEndOffset, boat.angle);
	return vec3_add(cannonEndOffset, boat.pos);
} 

bool boat_ballCollision(void* data, void* args) {
	Boat* boat = data;
	CannonBall* ball = args;

	if (ball->collisionTimer <= 0.0) {
		if (circleCircleCollision(boat->boundingCircle, ball->boundingCircle)) {
			ball_hitObject(ball, &boat->health);
		}
	}
}

void drawBoat(void* data, void* args){
	Boat* boat = data;

	glPushMatrix();
	glTranslatef(boat->pos.x, boat->pos.y, boat->pos.z);
	glRotatef(rad2deg(boat->rotation.x), 1.0, 0.0, 0.0);
	glRotatef(rad2deg(boat->rotation.z), 0.0, 0.0, 1.0);
	glRotatef(rad2deg(boat->rotation.y), 0.0, 1.0, 0.0);

	setMaterial(BOAT_COLOR, 0.5, 10); 

	//drawTrapezoid(0.15, 0.1, 0.05, (Vec2) { 0.0, 0.0 });
	Cuboid hullCuboid = {
		(Vec3) { -0.075, -0.025, -0.05 },
		(Vec3) { 0.075, 0.025, 0.05 }
	};
	drawCuboid(hullCuboid, NULL);

	Cuboid bridgeCuboid = {
		(Vec3) { -0.03, 0.025, -0.03 },
		(Vec3) { 0.03, 0.06, 0.03 }
	};
	drawCuboid(bridgeCuboid, NULL);

	glPushMatrix();
	glTranslatef(cannonOffset.x, cannonOffset.y, 0.0);
	glRotatef(-90.0 + rad2deg(boat->cannonAngle), 0.0, 0.0, 1.0);

	Cuboid cannonCuboid = {
		(Vec3) { -0.007, 0.0, -0.007 },
		(Vec3) { 0.007, BOAT_CANNON_LENGTH, 0.007 }
	};
	drawCuboid(cannonCuboid, NULL);

	glPopMatrix();

	//Draw collision circle
	//glColor4f(0.0, 1.0, 0.0, 0.3);
	//drawCircle((Position) { 0.0, 0.0 }, boatRadius);

	glPopMatrix();
}