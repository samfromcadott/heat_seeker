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

void shoot_ball(Player player, HSE::Position& position, HSE::Rotation& rotation) {
	if ( !IsMouseButtonPressed(0) ) return;

	glm::vec3 launch_point = ( glm::vec3(camera.target.x, camera.target.y, camera.target.z) - glm::vec3(position) ) * 0.5f;
	launch_point += position;
	glm::vec3 vel = ( glm::vec3(camera.target.x, camera.target.y, camera.target.z) - glm::vec3(position) ) * 5.0f;

	flecs::entity ball = Game.entity();

	ball.add<HSE::Model>();
	ball.add<Position>();
	ball.add<Rotation>();
	ball.add<Velocity>();
	ball.add<Body>();

	ball.set<Position>( glm::vec3(launch_point) );
	ball.set<Velocity>( glm::vec3(vel) );
	ball.set<HSE::Model>( {&model_files["ball"], 0, 0} );
	ball.set<Body>( Body(Game.get_mut<PhysicsEngine>(), ball, JPH::BodyCreationSettings(
		new JPH::SphereShape(0.05),
		JPH::RVec3::sZero(),
		JPH::Quat::sIdentity(),
		JPH::EMotionType::Dynamic,
		Layers::MOVING
	)));
}
