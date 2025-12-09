#include <algorithm>
#include <heat_seeker.hh>

#include "components.hh"
#include "systems.hh"

using namespace HSE;

void mouse_look(Player player, Position& position, Rotation& rotation) {
	// Get mouse movement
	Vector2 delta = GetMouseDelta();

	// Calculate the horizontal and vertical rotation
	glm::vec3 euler = glm::eulerAngles( glm::quat(rotation) );
	euler.z -= delta.x * GetFrameTime() * 0.1;
	euler.y -= delta.y * GetFrameTime() * 0.1;
	euler.y = std::clamp(euler.y, -1.5f, 1.5f);
	rotation = glm::quat(euler);

	// Update view
	glm::vec3 vector_h( cos(euler.z), sin(euler.z), 0.0f );
	glm::vec3 vector_v( cos(euler.y), 0.0f, sin(euler.y) );
	glm::vec3 camera_target( vector_h.x*vector_v.x, vector_h.y*vector_v.x, vector_v.z );
	camera_target += position;

	camera.target = {camera_target.x,camera_target.y,camera_target.z};
	camera.position = {position.x,position.y,position.z};
}
