#include "island.h"
#include "draw.h"
#include "config.h"
#include "cannonBall.h"
#include "camera.h"

Color island_color = { 1.0, 1.0, 0.0 };

static Vec3 cannonOffset = { 0.0, -0.02, 0.0 };
static float cannonLength = 0.2;

Island island_new() {
	return (Island) {
		.alpha = M_PI / 4.0,
		.theta = 0.0,
		.firePower = 1.2,
		.health = ISLAND_MAX_HEALTH,
		.cooldown = 0.0
	};
}

Cuboid island_getBoundingBox() {
	return (Cuboid){
		(Vec3) { -0.15, -1.0, -0.15 },
		(Vec3) { 0.15, 0.0, 0.15 }
	};
}

Vec3 island_getCannonEnd(Island island) {
	Vec3 cannonVector = vec3_polar2vec(island.alpha, island.theta, cannonLength);
	Vec3 cannonEndOffset = vec3_add(cannonVector, cannonOffset);

	return cannonEndOffset;
}

Vec3 island_getFireVelocity(Island island) {
	return vec3_polar2vec(island.alpha, island.theta, island.firePower);
}

void drawIsland(Island island, GLuint texture, Scene* scene) {
	disableLighting();

	//Draw cannon trajectory
	drawPath(island_getCannonEnd(island),
		island_getFireVelocity(island),
		scene);

	enableLighting();

	setMaterial((Vec4) { 1.0, 1.0, 1.0, 1.0 }, 0.3, 50);

	drawCuboid(island_getBoundingBox(), texture);

	glPushMatrix();
	glTranslatef(cannonOffset.x, cannonOffset.y, 0.0);
	glRotatef(rad2deg(island.theta), 0.0, 1.0, 0.0);
	glRotatef(rad2deg(island.alpha) - 90.0, 0.0, 0.0, 1.0);

	setMaterial(ISLAND_COLOR, 0.3, 50);
	Cuboid cannonCuboid = {
		(Vec3) { -0.02, 0.0, -0.02 },
		(Vec3) { 0.02, 0.2, 0.02 },
	};
	drawCuboid(cannonCuboid, NULL);

	glPopMatrix();
}