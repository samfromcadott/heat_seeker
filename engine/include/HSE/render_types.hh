#pragma once

#include <functional>
#include <raylib.h>

namespace HSE {

struct ModelData {
	::Model model;
	ModelAnimation* animations = nullptr;
	int animation_count = 0;
	std::map<std::string, int> anim_names;

	ModelData() = default;
	ModelData(const std::string& filename);
};

struct Model {
	HSE::Asset<ModelData> data;
	int animation = 0;
	int frame = 0;

	Model() = default;
	Model(const std::string& filename);

	void play(const std::string& anim_name);
};

struct ModelOptions {
	std::string file;
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
