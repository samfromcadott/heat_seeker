#include <algorithm>
#include <heat_seeker.hh>

#include "components.hh"
#include "systems.hh"
#include "weapon.hh"

using namespace HSE;

void mouse_look(PlayerCamera& player_camera, Position& position, Rotation& rotation) {
	// Get mouse movement
	Vector2 delta = GetMouseDelta();

	// Update camera rotation
	player_camera.rotation.z -= delta.x * GetFrameTime() * 0.1;
	player_camera.rotation.y -= delta.y * GetFrameTime() * 0.1;
	player_camera.rotation.y = std::clamp(player_camera.rotation.y, -1.5f, 1.5f);

	rotation = quat( vec3(0.0,0.0,player_camera.rotation.z) ); // Update player rotation

	// Update view
	vec3 vector_h( cos(player_camera.rotation.z), sin(player_camera.rotation.z), 0.0f );
	vec3 vector_v( cos(player_camera.rotation.y), 0.0f, sin(player_camera.rotation.y) );

	vec3 camera_target( vector_h.x*vector_v.x, vector_h.y*vector_v.x, vector_v.z );
	camera_target += vec3(position) + player_camera.offset;

	auto camera_position = vec3(position) + player_camera.offset;

	camera.target = glm_to_raylib(camera_target);
	camera.position = glm_to_raylib(camera_position);
}

void player_movement(Player player, HSE::Velocity& velocity, GroundMovement& movement, HSE::Rotation& rotation) {
	vec3 input_dir = vec3(0,0,0);
	if ( IsKeyDown(KEY_W) ) input_dir.x += 1.0;
	if ( IsKeyDown(KEY_S) ) input_dir.x -= 1.0;
	if ( IsKeyDown(KEY_A) ) input_dir.y += 1.0;
	if ( IsKeyDown(KEY_D) ) input_dir.y -= 1.0;

	input_dir = quat(rotation) * input_dir;
	input_dir.z = 0.0;
	float input_mag = length(input_dir);

	if (input_mag != 0) input_dir = normalize(input_dir);

	movement.direction = input_dir;
}

void ground_movement(HSE::Position& position, HSE::Velocity& velocity, HSE::CharacterBody& body, GroundMovement& movement) {
	float a;
	if ( movement.direction != vec3(0,0,0) )
		a = body.on_floor()? movement.acceleration : movement.acceleration_air;
	else
		a = body.on_floor()? movement.decceleration : movement.decceleration_air;

	auto wish_vel = movement.direction * movement.max_speed;
	vec3 v = mix(vec3(velocity), wish_vel, GetFrameTime() * a);

	velocity.x = v.x;
	velocity.y = v.y;
}

void player_jump(Player player, HSE::CharacterBody& body, HSE::Velocity& velocity, const Jump& jump) {
	if ( !IsKeyPressed(KEY_SPACE) or !body.on_floor() ) return;

	velocity.z = jump.speed;
}

void player_fire(Player player, HeldWeapon& weapon) {
	if ( !IsMouseButtonDown(0) ) return;
	fire_weapon(weapon.entity);
}
