#pragma once

#include <raylib.h>

namespace HSE {

struct ModelData {
	::Model model;
	ModelAnimation* animations;
	int animation_count;
};

struct Model {
	ModelData* data;
	int animation;
	int frame;
};

}
