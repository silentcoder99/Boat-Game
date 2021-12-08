#include <math.h>
#include <string.h>
#include "draw.h"
#include "util.h"
#include "config.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void drawAxes(float len) {
	glBegin(GL_LINES);
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(len, 0.0, 0.0);
	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, len, 0.0);
	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, len);
	glEnd();
}

void drawTangent(float m) {
	Vec2 norm = { 1.0, m };
	norm = vec2_normalise(norm);

	norm.x /= 5;
	norm.y /= 5;

	glBegin(GL_LINES);
	glColor3f(1.0, 0.0, 0.0);

	glVertex3f(0.0, 0.0, 1.0);
	glVertex3f(norm.x, norm.y, 1.0);

	glEnd();
}

void drawNormal(float m) {
	Vec2 norm = { 1.0 / m, 1.0 };
	norm = vec2_normalise(norm);

	norm.x /= 5;
	norm.y /= 5;

	glBegin(GL_LINES);
	glColor3f(0.0, 1.0, 0.0);

	glVertex3f(0.0, 0.0, 1.0);
	glVertex3f(norm.x, norm.y, 1.0);

	glEnd();
}

void drawTrapezoid(float a, float b, float h, Vec2 pos) {
	glBegin(GL_QUADS);

	float z = -0.9;

	glVertex3f(-a / 2.0 + pos.x, h / 2.0 + pos.y, z);
	glVertex3f(a / 2.0 + pos.x, h / 2.0 + pos.y, z);
	glVertex3f(b / 2.0 + pos.x, -h / 2.0 + pos.y, z);
	glVertex3f(-b / 2.0 + pos.x, -h / 2.0 + pos.y, z);

	glEnd();
}

void drawRect(Rect rect, Color color) {
	glBegin(GL_QUADS);
	glColor3f(color.r, color.g, color.b);

	glVertex3f(rect.min.x, rect.min.y, -0.9);
	glVertex3f(rect.max.x, rect.min.y, -0.9);
	glVertex3f(rect.max.x, rect.max.y, -0.9);
	glVertex3f(rect.min.x, rect.max.y, -0.9);

	glEnd();
}

void drawRectOutline(Rect rect, Color color, float width) {
	glColor3f(color.r, color.g, color.b);
	glLineWidth(width);
	glBegin(GL_LINE_LOOP);

	glVertex3f(rect.min.x, rect.min.y, -0.9);
	glVertex3f(rect.max.x, rect.min.y, -0.9);
	glVertex3f(rect.max.x, rect.max.y, -0.9);
	glVertex3f(rect.min.x, rect.max.y, -0.9);

	glEnd();
	glLineWidth(1.0);
}

void drawQuad(Vec3 pos, Vec2 size) {
	glPushMatrix();
	glTranslatef(pos.x, pos.y, pos.z);
	glBegin(GL_QUADS);
	glNormal3f(0.0, 1.0, 0.0);

	glVertex3f(-size.x / 2.0, 0.0, -size.y / 2.0);
	glVertex3f(-size.x / 2.0, 0.0, size.y / 2.0);
	glVertex3f(size.x / 2.0, 0.0, size.y / 2.0);
	glVertex3f(size.x / 2.0, 0.0, -size.y / 2.0);

	glEnd();
	glPopMatrix();
}

void drawCuboid(Cuboid cuboid, GLuint* texture) {
	Vec3 min = cuboid.min;
	Vec3 max = cuboid.max;

	Vec3 vertices[8] = {
		{ min.x, min.y, min.z },
		{ max.x, min.y, min.z },
		{ min.x, min.y, max.z },
		{ max.x, min.y, max.z },
		{ min.x, max.y, max.z },
		{ max.x, max.y, max.z },
		{ min.x, max.y, min.z },
		{ max.x, max.y, min.z },
	};

	int faces[6][4] = {
		{0, 1, 3, 2},
		{2, 3, 5, 4},
		{4, 5, 7, 6},
		{6, 7, 1, 0},
		{0, 2, 4, 6},
		{1, 3, 5, 7}
	};

	Vec3 normals[6] = {
		{0, -1, 0},
		{0, 0, 1},
		{0, 1, 0},
		{0, 0, -1},
		{-1, 0, 0},
		{1, 0, 0}
	};

	Vec2 texCoords[4] = {
		{0, 0},
		{1, 0},
		{1, 1},
		{0, 1}
	};

	if (texture != NULL) {
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture);
	}

	glBegin(GL_QUADS);
	for (int i = 0; i < 6; i++) {
		Vec3 normal = normals[i];
		glNormal3f(normal.x, normal.y, normal.z);

		for (int j = 0; j < 4; j++) {
			int index = faces[i][j];
			Vec3 vertex = vertices[index];
			if (texture != NULL) {
				Vec2 texCoord = texCoords[j];
				glTexCoord2f(texCoord.x, texCoord.y);
			}
			glVertex3f(vertex.x, vertex.y, vertex.z);
		}
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void drawCircle(Circle circle) {
	int num_segments = SPHERE_SEGMENTS;

	glPushMatrix();
	glTranslatef(circle.centre.x, circle.centre.y, circle.centre.z);
	
	glBegin(GL_TRIANGLE_FAN);

	glVertex3f(0.0, 0.0, 0.0);

	for (int i = 0; i <= num_segments; i++) {
		float angle = 2 * M_PI * i / num_segments;
		glVertex2f(cosf(angle) * circle.radius, sinf(angle) * circle.radius);
	}

	glEnd();

	glPopMatrix();
}

Vec3 getSphereNormal(float phi, float theta) {
	float x = cosf(theta) * sinf(phi);
	float y = cosf(phi);
	float z = sinf(theta) * sinf(phi);

	return (Vec3) { x, y, z };
}

void drawSphere(Circle circle, float tesselationFactor) {
	int tesselations = SPHERE_SEGMENTS * tesselationFactor;
	float phi_step = M_PI / tesselations;

	glPushMatrix();
	glTranslatef(circle.centre.x, circle.centre.y, circle.centre.z);

	for (int i = 0; i < tesselations; i++) {
		float phi = (float)i / tesselations * M_PI;

		glBegin(GL_QUAD_STRIP);

		for (int j = 0; j <= tesselations; j++) {
			float theta = (float)j / tesselations * 2.0 * M_PI;

			float x = cosf(theta) * sinf(phi) * circle.radius;
			float y = cosf(phi) * circle.radius;
			float z = sinf(theta) * sinf(phi) * circle.radius;

			float x2 = cosf(theta) * sinf(phi + phi_step) * circle.radius;
			float y2 = cosf(phi + phi_step) * circle.radius;
			float z2 = sinf(theta) * sinf(phi + phi_step) * circle.radius;

			Vec3 normal = getSphereNormal(phi, theta);
			glNormal3f(normal.x, normal.y, normal.z);
			glVertex3f(x, y, z);

			normal = getSphereNormal(phi + phi_step, theta);
			glNormal3f(normal.x, normal.y, normal.z);
			glVertex3f(x2, y2, z2);
		}

		glEnd();
	}

	glPopMatrix();
}

void drawHealthBar(Vec2 pos, float value, Color color) {
	glPushMatrix();
	glTranslatef(pos.x, pos.y, 0.0);

	Rect bar = {
		{ 0.0, 0.0 },
		{ 0.50, -0.05 }
	};

	//Outline
	drawRectOutline(bar, color, 5.0);

	//Fill
	bar.max.x *= value;
	drawRect(bar, color);

	glPopMatrix();
}

void drawText(Vec2 pos, Color color, char* str) {
	float lineSpacing = 0.06;

	glColor3f(color.r, color.g, color.b);
	glRasterPos2f(pos.x, pos.y);
	
	for (int i = 0; i < strlen(str); i++) {
		char c = str[i];
		if (c == '\n') {
			pos.y -= lineSpacing;
			glRasterPos2f(pos.x, pos.y);
		}
		else {
			glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c);
		}
	}
}

void drawVec(Vec3 pos, Vec3 magnitude, float scale, Color color) {
	magnitude = vec3_multiply(magnitude, scale);
	Vec3 endPoint = vec3_add(pos, magnitude);

	glBegin(GL_LINES);
	glColor3f(color.r, color.g, color.b);

	glVertex3f(pos.x, pos.y, pos.z);
	glVertex3f(endPoint.x, endPoint.y, endPoint.z);

	glEnd();
}

void setMaterial(Vec4 color, float specular, float shininess) {
	Vec4 specularColor = { specular, specular, specular, 1 };

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, &color);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, &specularColor);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
}

uint32_t loadTexture(const char* filename) {
	int width, height, components;
	unsigned char* data = stbi_load(filename, &width, &height, &components, STBI_rgb);
	if (!data) {
		perror("fopen");
	}

	glPushAttrib(GL_TEXTURE_BIT);

	unsigned int id;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	//glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	glPopAttrib();

	return id;
}