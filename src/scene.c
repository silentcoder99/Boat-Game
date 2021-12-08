#include "scene.h"
#include "cannonBall.h"
#include "boat.h"
#include "island.h"

Scene scene_new() {
	Scene newScene;
	newScene.timeData.lastT = 0.0;
	newScene.timeData.t = 0.0;
	newScene.islandBoundingBox = island_getBoundingBox();
	list_init(&newScene.cannonBalls, freeCannonBall);
	list_init(&newScene.boats, freeBoat);
	newScene.tesselationFactor = 1.0;
	newScene.score = 0;

	return newScene;
}