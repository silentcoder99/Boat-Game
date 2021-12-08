#include "config.h"

Settings settings_new() {
	return (Settings) {
		.wireframe = false,
		.normals = false,
		.tangents = false,
		.lighting = true,
		.animateWater = true,
		.gameOver = false,
		.aspectRatio = 1.0
	};
}