#include <math.h>
#include "util.h"

float rad2deg(float angle) {
	return angle * 180.0 / M_PI;
}

float deg2rad(float angle) {
	return angle / 180.0 * M_PI;
}

float clamp(float val, float min, float max) {
	if (val > max) {
		return max;
	}
	else if (val < min) {
		return min;
	}
	else {
		return val;
	}
}

Vec2 vec2_normalise(Vec2 vec) {
	Vec2 newVec;
	float magnitude = sqrt(pow(vec.x, 2) + pow(vec.y, 2));
	newVec.x = vec.x / magnitude;
	newVec.y = vec.y / magnitude;

	return newVec;
}

Vec2 vec2_rotate(Vec2 pos, float angle) {
	return (Vec2) {
		pos.x * cosf(angle) - pos.y * sinf(angle),
			pos.x * sinf(angle) + pos.y * cosf(angle)
	};
}

float vec2_magnitude(Vec2 vec) {
	return sqrt(pow(vec.x, 2) + pow(vec.y, 2));
}

float vec2_arg(Vec2 vec) {
	return atan2f(vec.x, vec.y);
}

Vec2 vec2_polar2vec(float argument, float magnitude) {
	return (Vec2) { 
		magnitude * cosf(argument),
		magnitude * sinf(argument)
	};
}

Vec2 vec2_add(Vec2 posA, Vec2 posB) {
	return (Vec2) {
		posA.x + posB.x,
		posA.y + posB.y
	};
}

Vec2 vec2_subtract(Vec2 a, Vec2 b) {
	return (Vec2) {
		a.x - b.x,
			a.y - b.y
	};
}

Vec3 vec3_add(Vec3 a, Vec3 b) {
	return (Vec3) {
		a.x + b.x,
			a.y + b.y,
			a.z + b.z
	};
}

Vec3 vec3_subtract(Vec3 a, Vec3 b) {
	return (Vec3) {
		a.x - b.x,
			a.y - b.y,
			a.z - b.z
	};
}

Vec3 vec3_multiply(Vec3 vec, float scalar) {
	return (Vec3) {
		vec.x* scalar,
			vec.y* scalar,
			vec.z* scalar
	};
}

float vec3_magnitude(Vec3 vec) {
	return sqrt(pow(vec.x, 2) + pow(vec.y, 2) + pow(vec.z, 2));
}

Vec3 vec3_normalise(Vec3 vec) {
	Vec3 newVec;
	float magnitude = vec3_magnitude(vec);
	newVec.x = vec.x / magnitude;
	newVec.y = vec.y / magnitude;
	newVec.z = vec.z / magnitude;

	return newVec;
}

Vec3 vec3_polar2vec(float alpha, float theta, float magnitude) {
	return (Vec3) {
		magnitude * cosf(theta) * cosf(alpha),
		magnitude * sinf(alpha),
		magnitude * -sinf(theta) * cosf(alpha)
	};
}

Vec3 vec3_yaw(Vec3 vec, float angle) {
	return (Vec3) {
		vec.x * cosf(angle) - vec.z * sinf(angle),
		vec.y,
		vec.x * sinf(angle) + vec.z * cosf(angle)
	};
}

Vec3 vec3_pitch(Vec3 vec, float angle) {
	return (Vec3) {
		vec.x * cosf(angle) - vec.y * sinf(angle),
		vec.x * sinf(angle) + vec.y * cosf(angle),
		vec.z
	};
}

Vec3 vec3_roll(Vec3 vec, float angle) {
	return (Vec3) {
		vec.x,
		vec.z * cosf(angle) - vec.y * sinf(angle),
		vec.z * sinf(angle) + vec.y * cosf(angle)
	};
}