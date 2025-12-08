#pragma once

#include <raylib.h>

namespace HSE {

struct ModelData {
	::Model model;
	ModelAnimation* animations = nullptr;
	int animation_count = 0;

	~ModelData() {
		UnloadModel(model);
		UnloadModelAnimations(animations, animation_count);
	}
};

struct Model {
	ModelData* data;
	int animation;
	int frame;
};

}
