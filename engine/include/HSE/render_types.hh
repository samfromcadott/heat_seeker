#pragma once

#include <functional>
#include <raylib.h>
#include <rlgl.h>

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
	std::string shader = "";
};

struct Light {
	vec4 color = {1.0,1.0,1.0,1.0};
	float power = 1.0;
};

extern std::function<void(void)> ui_function;
extern Shader gouraud_shader;
extern std::map<std::string, Shader> shaders;
extern std::string default_shader;
extern Color clear_color;

}
