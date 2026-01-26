#pragma once

// STD Libraries
#include <iostream>
#include <map>

#include <raylib.h>
#include <raymath.h>
#include <flecs.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/ext/quaternion_trigonometric.hpp>
#include <glm/gtx/rotate_vector.hpp>

// Merge glm namespace into HSE
namespace HSE {

inline namespace glm { using namespace ::glm; }

}

#include "HSE/basic_types.hh"
#include "HSE/physics_types.hh"
#include "HSE/physics_systems.hh"
#include "HSE/render_types.hh"
#include "HSE/render_systems.hh"
#include "HSE/script.hh"
#include "HSE/modules.hh"

namespace HSE {

extern flecs::world Game;
extern Camera3D camera;
extern std::map<std::string, ModelData> model_files;

void init(const std::string& title, const int width, const int height);
void quit();

}
