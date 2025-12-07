#include <heat_seeker.hh>

using namespace HSE;

Body::Body() {

}

Body::Body(PhysicsEngine& engine, flecs::entity owner, const JPH::BodyCreationSettings& settings) {

}

Body::~Body() {

}

void Body::set_position(const glm::vec3& position) {

}

glm::vec3 Body::get_position() {
	return glm::vec3(0,0,0);
}

void Body::set_rotation(const glm::quat& rotation) {

}

glm::quat Body::get_rotation() {
	return glm::quat(1,0,0,0);
}

void Body::set_velocity(const glm::vec3& velocity) {

}

glm::vec3 Body::get_velocity() {
	return glm::vec3(0,0,0);
}

// flecs::entity Body::get_owner() {
//
// }
