#ifndef UTIL_H
#define UTIL_H

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

typedef struct {
	float x;
	float y;
} Vec2;

typedef struct {
	float x;
	float y;
	float z;
} Vec3;

typedef struct {
	float x;
	float y;
	float z;
	float w;
} Vec4;

typedef struct {
	float r;
	float g;
	float b;
} Color;

typedef struct { Vec3 centre; float radius; } Circle;
typedef struct { Vec2 min; Vec2 max; } Rect;
typedef struct { Vec3 min; Vec3 max; } Cuboid;

float rad2deg(float angle);
float deg2rad(float angle);

float clamp(float val, float min, float max);

Vec2 vec2_normalise(Vec2 vec);
Vec2 vec2_rotate(Vec2 pos, float angle);
float vec2_magnitude(Vec2 vec);
float vec2_arg(Vec2 vec);
Vec2 vec2_polar2vec(float argument, float magnitude);
Vec2 vec2_add(Vec2 posA, Vec2 posB);
Vec2 vec2_subtract(Vec2 a, Vec2 b);

Vec3 vec3_add(Vec3 a, Vec3 b);
Vec3 vec3_subtract(Vec3 a, Vec3 b);
Vec3 vec3_multiply(Vec3 vec, float scalar);
float vec3_magnitude(Vec3 vec);
Vec3 vec3_normalise(Vec3 vec);
Vec3 vec3_polar2vec(float alpha, float theta, float magnitude);
Vec3 vec3_yaw(Vec3 vec, float angle);
Vec3 vec3_pitch(Vec3 vec, float angle);
Vec3 vec3_roll(Vec3 vec, float angle);

#endif