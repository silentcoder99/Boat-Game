#include <math.h>
#include "water.h"
#include "util.h"
#include "draw.h"
#include "config.h"

Wave waterWave = {
	.wavelength = 1.0,
	.amplitude = 1.0 / 8.0,
	.c = -0.2
};

float waterFunction(Vec2 pos, float t) {
	float xWave = sinf(M_PI * 2.0 * pos.x / waterWave.wavelength + t)
		* waterWave.amplitude + waterWave.c;

	float zWave = sinf(M_PI * 2.0 * pos.y / waterWave.wavelength + t)
		* waterWave.amplitude + waterWave.c;

	return xWave + zWave;
}

Vec2 waterGradient(Vec2 pos, float t) {
	float xGradient = cosf(M_PI * 2.0 * pos.x / waterWave.wavelength + t)
		* waterWave.amplitude * M_PI * 2.0 / waterWave.wavelength;

	float zGradient = cosf(M_PI * 2.0 * pos.y / waterWave.wavelength + t)
		* waterWave.amplitude * M_PI * 2.0 / waterWave.wavelength;

	return (Vec2) { xGradient, zGradient };
}

Vec3 getWaterNormal(Vec2 pos, float t) {
	Vec2 m = waterGradient(pos, t);
	Vec3 normal = { -m.x, 1.0, -m.y };
	normal = vec3_normalise(normal);

	return normal;
}

Vec3 getXTangent(Vec2 pos, float t) {
	Vec2 m = waterGradient(pos, t);
	Vec3 tangent = { 1.0, m.x, 0.0 };
	tangent = vec3_normalise(tangent);

	return tangent;
}

Vec3 getZTangent(Vec2 pos, float t) {
	Vec2 m = waterGradient(pos, t);
	Vec3 tangent = { 0.0, m.y, 1.0 };
	tangent = vec3_normalise(tangent);

	return tangent;
}

void drawWater(float time, int tesselations, float size) {
	float step = size / tesselations;
	setMaterial(WATER_COLOR, 1, 126);

	for (int i = 0; i < tesselations; i++) {
		glBegin(GL_TRIANGLE_STRIP);

		for (int j = 0; j <= tesselations; j++) {
			float x = -size / 2.0 + (float)i / tesselations * size;
			float z = -size / 2.0 + (float)j / tesselations * size;

			float y = waterFunction((Vec2) { x, z }, time);
			float y2 = waterFunction((Vec2) { x + step, z }, time);

			Vec3 normal = getWaterNormal((Vec2) { x, z }, time);
			glNormal3f(normal.x, normal.y, normal.z);
			glVertex3f(x, y, z);

			normal = getWaterNormal((Vec2) { x + step, z }, time);
			glNormal3f(normal.x, normal.y, normal.z);
			glVertex3f(x + step, y2, z);
		}

		glEnd();
	}
}

void drawNormals(float time, int tesselations, float size) {
	for (int i = 0; i <= tesselations; i++) {
		for (int j = 0; j <= tesselations; j++) {
			float x = -size / 2.0 + (float)i / tesselations * size;
			float z = -size / 2.0 + (float)j / tesselations * size;
			float y = waterFunction((Vec2) { x, z }, time);

			Vec3 normal = getWaterNormal((Vec2) { x, z }, time);
			Color yellow = { 1, 1, 0 };

			drawVec((Vec3) { x, y, z }, normal, 0.025, yellow);
		}
	}
}

void drawTangents(float time, int tesselations, float size) {
	for (int i = 0; i <= tesselations; i++) {
		for (int j = 0; j <= tesselations; j++) {
			float x = -size / 2.0 + (float)i / tesselations * size;
			float z = -size / 2.0 + (float)j / tesselations * size;
			float y = waterFunction((Vec2) { x, z }, time);

			Vec3 xTangent = getXTangent((Vec2) { x, z }, time);
			Vec3 zTangent = getZTangent((Vec2) { x, z }, time);

			Color cyan = { 0, 1, 1 };
			Color magenta = { 1, 0, 1 };

			drawVec((Vec3) { x, y, z }, xTangent, 0.025, cyan);
			drawVec((Vec3) { x, y, z }, zTangent, 0.025, magenta);
		}
	}
}