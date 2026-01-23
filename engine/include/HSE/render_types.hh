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
	Color debug_color;

	Model() = default;
	Model(ModelData* data, int animation, int frame) {
		this->data = data;
		this->animation = animation;
		this->frame = frame;
	}
	Model(const std::string& filename);
};

class LevelModel {
public:
	LevelModel();
	LevelModel(flecs::world& world, const std::string& filename);
	~LevelModel();

private:
	::Model model;

};

extern std::function<void(void)> ui_function;
extern Shader gouraud_shader;
extern Texture2D uv_debug_texture;

}
