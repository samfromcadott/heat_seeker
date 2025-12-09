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
#include <glm/ext/quaternion_trigonometric.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include "HSE/basic_types.hh"
#include "HSE/physics_types.hh"
#include "HSE/physics_systems.hh"
#include "HSE/render_types.hh"
#include "HSE/render_systems.hh"

namespace HSE {

extern flecs::world Game;
extern Camera3D camera;
extern std::map<std::string, ModelData> model_files;

}
