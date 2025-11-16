cmake_minimum_required(VERSION 3.29)
set(CMAKE_CXX_STANDARD 20)

project(heat_seeker)

set(USE_STATIC_MSVC_RUNTIME_LIBRARY OFF)
add_subdirectory(3rdparty/SDL)
include_directories(3rdparty/SDL/include)

add_executable(heat_seeker
	engine/main.cc
)

target_link_libraries(heat_seeker SDL3::SDL3)
