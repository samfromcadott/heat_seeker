cmake_minimum_required(VERSION 3.29)
set(CMAKE_CXX_STANDARD 20)

project(heat_seeker)

set(USE_STATIC_MSVC_RUNTIME_LIBRARY OFF)
add_subdirectory(3rdparty/raylib)
add_subdirectory(3rdparty/flecs)
add_subdirectory(3rdparty/Jolt/Build)
add_subdirectory(3rdparty/physfs)

include_directories(3rdparty)
include_directories(3rdparty/glm)
include_directories(engine/include)
include_directories(3rdparty/physfs/src)

add_library(heat_seeker
	engine/src/hse.cc
	engine/src/modules.cc
	engine/src/file.cc
	engine/src/data_parser.cc
	engine/src/level.cc

	# Render
	engine/src/render/render.cc
	engine/src/render/models.cc

	# Physics
	engine/src/physics/body.cc
	engine/src/physics/engine.cc
	engine/src/physics/listener.cc
	engine/src/physics/systems.cc
	engine/src/physics/character.cc
	engine/src/physics/character_listener.cc
	engine/src/physics/shape.cc
)

target_link_libraries(heat_seeker raylib flecs_static Jolt physfs-static)

add_executable(rpg
	rpg/src/main.cc
	rpg/src/player.cc
	rpg/src/weapon.cc
	rpg/src/combat.cc
	rpg/src/monster.cc
)

target_link_libraries(rpg heat_seeker)
