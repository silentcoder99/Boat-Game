#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <ctype.h>
#include <time.h>
#include "list.h"
#include "util.h"
#include "cannonBall.h"
#include "config.h"
#include "draw.h"
#include "collision.h"
#include "boat.h"
#include "water.h"
#include "island.h"
#include "time.h"
#include "camera.h"
#include "scene.h"

#if _WIN32
#   include <Windows.h>
#endif
#if __APPLE__
#   include <OpenGL/gl.h>
#   include <OpenGL/glu.h>
#   include <GLUT/glut.h>
#else
#   include <GL/gl.h>
#   include <GL/glu.h>
#   include <GL/glut.h>
#endif

#define LIGHTING

Scene scene;
Settings settings;

Camera camera;
Mouse mouse;

Wave water;

List defences;

float spawnTimer = SPAWN_TIME;

Island island;
GLuint islandTexture;

bool keyIsDown[256];
bool specialKeyIsDown[256];

Vec2 cannonOffset = { 0.04, 0.022 };

void drawBalls() {
	list_forEach(&scene.cannonBalls, drawBall, &scene.tesselationFactor);
	list_forEach(&defences, drawBall, &scene.tesselationFactor);
}

void drawPaths() {
	list_forEach(&scene.cannonBalls, ball_drawPath, &scene);
}

void drawBoats() {
	list_forEach(&scene.boats, drawBoat, NULL);
}

void drawHealthBars() {
	drawHealthBar((Vec2) { -0.93, 0.93 }, island.health / ISLAND_MAX_HEALTH,
		island_color);
}

void drawScore() {
	char scoreText[30];
	snprintf(scoreText, 30, "Score: %d", scene.score);
	drawText((Vec2) { -0.93, 0.80 }, (Color) { 1.0, 1.0, 0.0 }, scoreText);
}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	setPerspective(settings.aspectRatio);
	pushCameraMatrix(camera);

	if (settings.lighting) {
		enableLighting();

		//Set light
		Vec4 lightPos = { 1, 1, 1, 0 };
		glLightfv(GL_LIGHT0, GL_POSITION, &lightPos);
	}
	else {
		disableLighting();
	}

	drawBoats();
	drawIsland(island, islandTexture, &scene);
	drawWater(scene.timeData.t, WATER_SEGMENTS * scene.tesselationFactor,
		WATER_SIZE);
	drawBalls();

	disableLighting();

	drawAxes(1.0);
	drawPaths();

	if (settings.normals) {
		drawNormals(scene.timeData.t, WATER_SEGMENTS * scene.tesselationFactor,
			WATER_SIZE);
	}
	if (settings.tangents) {
		drawTangents(scene.timeData.t, WATER_SEGMENTS * scene.tesselationFactor,
			WATER_SIZE);
	}

	glPopMatrix();

	setOrtho(settings.aspectRatio);

	drawHealthBars();
	drawScore();
	drawFrameInfo(scene.timeData);

	if (settings.gameOver) {
		drawText((Vec2) { -0.145, 0.5 },
			(Color) {
			1.0, 1.0, 1.0
		},
			"GAME OVER");
	}

	/* Always check for errors! */
	int err;
	while ((err = glGetError()) != GL_NO_ERROR)
		printf("display: %s\n", gluErrorString(err));

	glutSwapBuffers();
}

void addBall(CannonBall* ball) {
	if (!ball->isDefence) {
		list_pushFront(&scene.cannonBalls, ball);
		ball->list = &scene.cannonBalls;
	}
	else {
		list_pushFront(&defences, ball);
		ball->list = &defences;
	}
}

void fireIslandCannon(bool isDefence) {
	if (island.cooldown <= 0.0) {
		Vec3 pos = island_getCannonEnd(island);
		Vec3 vel = island_getFireVelocity(island);
		CannonBall* ball = ball_new(pos, vel, isDefence);
		addBall(ball);
		island.cooldown = COOLDOWN_TIME;
	}
}

void keyDown(unsigned char key, int x, int y) {
	switch (key) {
		case 27:
			exit(EXIT_SUCCESS);
			break;
		case 'p': {
			settings.wireframe = !settings.wireframe;
			break;
		}
		case 'n': {
			settings.normals = !settings.normals;
			settings.tangents = !settings.tangents;
			break;
		}
		case '+': {
			if (scene.tesselationFactor < 4.0) {
				scene.tesselationFactor *= 2;
			}
			break;
		}
		case '-': {
			if (scene.tesselationFactor > 0.25) {
				scene.tesselationFactor /= 2;
			}
			break;
		}
		case 'h': {
			settings.animateWater = !settings.animateWater;
		}
		case 'l': {
			settings.lighting = !settings.lighting;
		}
	}

	keyIsDown[key] = true;
}

void keyUp(unsigned char key, int x, int y) {
	/* It's possible for the user to push an uppercase key
	   then release it as a lowercase key and vice versa.
	
	   So when a key is released, release both uppercase and
	   lowercase keys.
	*/
	char lower = tolower(key);
	char upper = toupper(key);

	keyIsDown[lower] = false;
	keyIsDown[upper] = false;
}

void specialKeyDown(int key, int x, int y) {
	specialKeyIsDown[key] = true;
}

void specialKeyUp(int key, int x, int y) {
	specialKeyIsDown[key] = false;
}

void updateBall(void* data, void* args) {
	CannonBall* ball = data;

	Vec3 posStep = vec3_multiply(ball->vel, scene.timeData.dt);
	ball->pos = vec3_add(ball->pos, posStep);

	ball->vel.y -= GRAVITY * scene.timeData.dt;

	ball->collisionTimer -= scene.timeData.dt;

	if (ball->isDefence) {
		ball->lifeTime += scene.timeData.dt;

		if (ball->lifeTime > DEFENCE_LIFETIME) {
			list_remove(&defences, data);
			return;
		}
	}

	ball->boundingCircle = (Circle){
		ball->pos,
		BALL_RADIUS + ball->lifeTime * SPREAD_RATE
	};

	//Sphere-water collision
	if (ball->pos.y < waterFunction((Vec2) { ball->pos.x, ball->pos.z },
			scene.timeData.t)) {
		ball_hitObject(ball, NULL);
		return;
	}

	//Ball-boat collision
	list_forEach(&scene.boats, boat_ballCollision, ball);

	//Ball-island collision
	if (circleCuboidCollision(ball->boundingCircle, island_getBoundingBox())) {
		ball_hitObject(ball, &island.health);
		return;
	}

	//Ball-defence collision
	if (!ball->isDefence) {
		Node* currentNode = defences.head;

		while (currentNode != NULL) {
			Node* nextNode = currentNode->next;
			CannonBall* defence = currentNode->data;

			if (circleCircleCollision(defence->boundingCircle, ball->boundingCircle)) {
				ball_hitObject(ball, NULL);
				return;
			}

			currentNode = nextNode;
		}
	}
}

void idle() {
	updateTime(&scene.timeData, settings.animateWater);

	//Settings
	if (settings.wireframe) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	if (!settings.gameOver) {
		//Island controls
		if (keyIsDown['a']) {
			island.alpha += scene.timeData.dt;
		}
		if (keyIsDown['d']) {
			island.alpha -= scene.timeData.dt;
		}
		if (specialKeyIsDown[GLUT_KEY_LEFT]) {
			island.theta += scene.timeData.dt;
		}
		if (specialKeyIsDown[GLUT_KEY_RIGHT]) {
			island.theta -= scene.timeData.dt;
		}
		if (keyIsDown['w']) {
			island.firePower += scene.timeData.dt;
		}
		if (keyIsDown['s']) {
			island.firePower -= scene.timeData.dt;
		}

		if (keyIsDown[' ']) {
			fireIslandCannon(false);
		}
		if (keyIsDown['v']) {
			fireIslandCannon(true);
		}
	}

	//Boat updating
	list_forEach(&scene.boats, boat_update, &scene);

	//Boat spawning
	if (spawnTimer <= 0) {
		Boat* boat = boat_new();
		list_pushFront(&scene.boats, boat);

		spawnTimer = SPAWN_TIME;
	}
	spawnTimer -= scene.timeData.dt;

	island.cooldown -= scene.timeData.dt;
	island.alpha = clamp(island.alpha, 0, M_PI);
	island.firePower = clamp(island.firePower, 0.0, ISLAND_MAX_VELOCITY);
	if (island.health <= 0) {
		island.health = 0;
		settings.gameOver = true;
	}

	//Ball movement
	list_forEach(&scene.cannonBalls, updateBall, &scene);
	list_forEach(&defences, updateBall, &scene);

	glutPostRedisplay();
}

void mouseMotion(int x, int y) {
	Vec2 mouseDelta = vec2_subtract((Vec2) { x, y }, mouse.lastMousePos);

	if (mouse.leftButtonDown) {
		camera.rotation.y += mouseDelta.x;
		camera.rotation.x += mouseDelta.y;
	}

	if (mouse.rightButtonDown) {
		camera.zoom += mouseDelta.y * 0.01;
	}

	mouse.lastMousePos = (Vec2){ x, y };
}

void mouseFunc(int button, int state, int x, int y) {

	if (state == GLUT_DOWN) {
		//Mouse wheel scrolling
		if (button == 3) {
			camera.zoom -= 0.05;
		}
		if (button == 4) {
			camera.zoom += 0.05;
		}

		mouse.lastMousePos = (Vec2){ x, y };

		if (button == GLUT_LEFT_BUTTON) {
			mouse.leftButtonDown = true;
		}
		if (button == GLUT_RIGHT_BUTTON) {
			mouse.rightButtonDown = true;
		}
	}

	if (state == GLUT_UP) {
		if (button == GLUT_LEFT_BUTTON) {
			mouse.leftButtonDown = false;
		}
		if (button == GLUT_RIGHT_BUTTON) {
			mouse.rightButtonDown = false;
		}
	}
}

void reshapeFunc(int width, int height) {
	glViewport(0, 0, width, height);
	settings.aspectRatio = (float)glutGet(GLUT_WINDOW_WIDTH) /
		(float)glutGet(GLUT_WINDOW_HEIGHT);
}

void init() {
	//Initialize random number generator
	srand(time(NULL));

	//Disable auto key repeating
	glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);

	//Enable alpha blending
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	//Initialise keyIsDown map
	for (int i = 0; i < 256; i++) {
		keyIsDown[i] = false;
	}

	scene = scene_new();

	mouse.lastMousePos = (Vec2){ 0.0, 0.0 };

	camera.rotation = (Vec3){ 0.0, 0.0, 0.0 };
	camera.zoom = 0.4;

	list_init(&defences, freeCannonBall);

	island = island_new();

	settings = settings_new();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("Assignment 2");

	init();

	glutReshapeWindow(500, 500);
	glutMouseFunc(mouseFunc);
	glutMotionFunc(mouseMotion);
	glutReshapeFunc(reshapeFunc);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyDown);
	glutKeyboardUpFunc(keyUp);
	glutSpecialFunc(specialKeyDown);
	glutSpecialUpFunc(specialKeyUp);
	glutIdleFunc(idle);

	islandTexture = loadTexture("island_top.png");

	glutMainLoop();

	return EXIT_SUCCESS;
}
