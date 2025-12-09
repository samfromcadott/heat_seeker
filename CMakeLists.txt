cmake_minimum_required(VERSION 3.29)
set(CMAKE_CXX_STANDARD 20)

project(heat_seeker)

set(USE_STATIC_MSVC_RUNTIME_LIBRARY OFF)
add_subdirectory(3rdparty/raylib)
add_subdirectory(3rdparty/flecs)
add_subdirectory(3rdparty/Jolt/Build)

include_directories(3rdparty/glm)
include_directories(engine/include)

add_library(heat_seeker
	# Render
	engine/src/render/render.cc
	engine/src/render/models.cc

	# Physics
	engine/src/physics/body.cc
	engine/src/physics/engine.cc
	engine/src/physics/filter.cc
	engine/src/physics/interface.cc
	engine/src/physics/listener.cc
	engine/src/physics/systems.cc
)

target_link_libraries(heat_seeker raylib flecs_static Jolt)

add_executable(rpg
	rpg/src/main.cc
	rpg/src/player.cc
)

target_link_libraries(rpg heat_seeker)
