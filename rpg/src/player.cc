#include <algorithm>
#include <heat_seeker.hh>

#include "components.hh"
#include "systems.hh"

using namespace HSE;

void mouse_look(PlayerCamera& player_camera, Position& position, Rotation& rotation) {
	// Get mouse movement
	Vector2 delta = GetMouseDelta();

	// Update camera rotation
	player_camera.rotation.z -= delta.x * GetFrameTime() * 0.1;
	player_camera.rotation.y -= delta.y * GetFrameTime() * 0.1;
	player_camera.rotation.y = std::clamp(player_camera.rotation.y, -1.5f, 1.5f);

	rotation = glm::quat( glm::vec3(0.0,0.0,player_camera.rotation.z) ); // Update player rotation

	// Update view
	glm::vec3 vector_h( cos(player_camera.rotation.z), sin(player_camera.rotation.z), 0.0f );
	glm::vec3 vector_v( cos(player_camera.rotation.y), 0.0f, sin(player_camera.rotation.y) );

	glm::vec3 camera_target( vector_h.x*vector_v.x, vector_h.y*vector_v.x, vector_v.z );
	camera_target += glm::vec3(position) + player_camera.offset;

	auto camera_position = glm::vec3(position) + player_camera.offset;

	camera.target = glm_to_raylib(camera_target);
	camera.position = glm_to_raylib(camera_position);
}

void player_movement(Player player, Position& position, Rotation& rotation, Velocity& velocity, CharacterBody& body, GroundMovement& movement) {
	glm::vec3 input_dir = glm::vec3(0,0,0);
	if ( IsKeyDown(KEY_W) ) input_dir.x += 1.0;
	if ( IsKeyDown(KEY_S) ) input_dir.x -= 1.0;
	if ( IsKeyDown(KEY_A) ) input_dir.y += 1.0;
	if ( IsKeyDown(KEY_D) ) input_dir.y -= 1.0;

	input_dir = glm::quat(rotation) * input_dir;
	input_dir.z = 0.0;
	float input_mag = glm::length(input_dir);

	if (input_mag != 0) input_dir = glm::normalize(input_dir);

	// auto v = glm::vec3(input_dir) * 4.0f;
	// velocity.x = v.x;
	// velocity.y = v.y;

	float a;
	if (input_mag != 0)
		a = body.on_floor()? movement.acceleration : movement.acceleration_air;
	else
		a = body.on_floor()? movement.decceleration : movement.decceleration_air;

	glm::vec3 v = glm::mix(glm::vec3(velocity), input_dir * movement.max_speed, GetFrameTime() * a);

	velocity.x = v.x;
	velocity.y = v.y;
}

void player_jump(Player player, HSE::CharacterBody& body, HSE::Velocity& velocity) {
	if ( !IsKeyPressed(KEY_SPACE) or !body.on_floor() ) return;

	velocity.z = 5.0;
}

void shoot_ball(Player player, HSE::Position& position, HSE::Rotation& rotation) {
	if ( !IsMouseButtonPressed(0) ) return;

	glm::vec3 offset = raylib_to_glm(camera.target) - raylib_to_glm(camera.position);
	glm::vec3 launch_point = offset * 0.5f;
	launch_point += raylib_to_glm(camera.position);
	glm::vec3 vel = offset * 10.0f;

	flecs::entity ball = Game.entity();

	ball.add<HSE::Model>();
	ball.add<Position>();
	ball.add<Rotation>();
	ball.add<Velocity>();
	ball.add<Body>();

	ball.set<Position>( glm::vec3(launch_point) );
	ball.set<Velocity>( glm::vec3(vel) );
	ball.set<Rotation>(rotation);
	ball.set<HSE::Model>( {&model_files["ball"], 0, 0} );
	ball.set<Body>( Body(Game, JPH::BodyCreationSettings(
		new JPH::SphereShape(0.05),
		JPH::RVec3::sZero(),
		JPH::Quat::sIdentity(),
		JPH::EMotionType::Dynamic,
		Layers::MOVING
	)));
}
