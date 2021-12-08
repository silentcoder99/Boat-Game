#include <stdio.h>
#include "cannonBall.h"
#include "config.h"
#include "draw.h"
#include "water.h"
#include "collision.h"

CannonBall* ball_new(Vec3 pos, Vec3 vel, bool isDefence) {
	CannonBall* ball = malloc(sizeof(CannonBall));
	if (ball == NULL) {
		printf("error: failed to alloc ball memory\n");
		return NULL;
	}

	ball->pos = pos;
	ball->vel = vel;
	ball->collisionTimer = BALL_COLLISION_TIME;
	ball->isDefence = isDefence;
	ball->lifeTime = 0.0;
	ball->boundingCircle = (Circle){
		pos,
		BALL_RADIUS
	};
	ball->list = NULL;

	return ball;
}

void ball_hitObject(CannonBall* ball, int* targetHealth) {
	if (!ball->isDefence) {
		if (targetHealth != NULL) {
			(*targetHealth)--;
		}
	}

	list_remove(ball->list, ball);
}

void freeCannonBall(void* cannonBall) {
	free(cannonBall);
}

void drawPath(Vec3 pos, Vec3 vel, Scene* scene) {
	Vec3 vertices[PATH_STEPS + 1];
	int numVertices = 1;

	float dt = 0.005;
	int segments = PATH_SEGMENTS * scene->tesselationFactor;

	Vec3 currentPos = pos;
	Vec3 currentVel = vel;
	vertices[0] = currentPos;

	for (int i = 0; i < PATH_STEPS; i++) {
		float waterHeight = waterFunction((Vec2) { currentPos.x, currentPos.z },
			scene->timeData.t);
		if (currentPos.y <= waterHeight ||
				pointInsideCuboid(currentPos, scene->islandBoundingBox)) {
			break;
		}

		Vec3 posStep = vec3_multiply(currentVel, dt);
		currentPos = vec3_add(currentPos, posStep);

		currentVel.y -= GRAVITY * dt;

		vertices[i + 1] = currentPos;
		numVertices++;
	}

	glBegin(GL_LINE_STRIP);

	int vertexStep = (PATH_STEPS + 1) / segments;
	for (int i = 0; i < numVertices; i += vertexStep) {
		Vec3 vertex = vertices[i];
		glVertex3f(vertex.x, vertex.y, vertex.z);
	}

	Vec3 lastVertex = vertices[numVertices - 1];
	glVertex3f(lastVertex.x, lastVertex.y, lastVertex.z);

	glEnd();
}

void ball_drawPath(void* data, void* arg) {
	CannonBall* ball = data;
	Scene* scene = arg;
	
	drawPath(ball->pos, ball->vel, scene);
}

void drawBall(void* data, void* arg) {
	CannonBall* ball = data;
	float* tesselationFactor = arg;

	Vec4 ballColor = { 0.5, 0.5, 0.5, 1.0 - ball->boundingCircle.radius * 3 };
	setMaterial(ballColor, 0.5, 1);
	drawSphere(ball->boundingCircle, *tesselationFactor);
}