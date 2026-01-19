#pragma once

#include <functional>
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
	ModelData* data = nullptr;
	int animation = 0;
	int frame = 0;

	Model() = default;
	Model(ModelData* data, int animation, int frame) {
		this->data = data;
		this->animation = animation;
		this->frame = frame;
	}
	Model(const std::string& filename);
};

extern std::function<void(void)> ui_function;

}
