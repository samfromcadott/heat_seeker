cmake_minimum_required(VERSION 3.29)
set(CMAKE_CXX_STANDARD 20)

project(heat_seeker)

set(USE_STATIC_MSVC_RUNTIME_LIBRARY OFF)
add_subdirectory(3rdparty/raylib)
add_subdirectory(3rdparty/flecs)

include_directories(3rdparty/glm)
include_directories(engine/include)

add_executable(heat_seeker
	engine/src/main.cc
	engine/src/render/render.cc
	engine/src/render/models.cc
)

target_link_libraries(heat_seeker raylib flecs_static)
