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
	float yaw = eulerAngles( quat(rotation) ).z;
	yaw -= delta.x * GetFrameTime() * 0.1;
	player_camera.pitch += delta.y * GetFrameTime() * 0.1;
	player_camera.pitch = std::clamp(player_camera.pitch, -1.5f, 1.5f);

	rotation = quat( vec3(0.0,0.0,yaw) ); // Update player rotation

	// Update view
	vec3 camera_target = quat( vec3(0, player_camera.pitch, yaw) ) * vec3(1,0,0);
	camera_target += vec3(position) + player_camera.offset;

	auto camera_position = vec3(position) + player_camera.offset;

	camera.target = glm_to_raylib(camera_target);
	camera.position = glm_to_raylib(camera_position);
}

void player_movement(Player player, HSE::Velocity& velocity, MoveDir& dir, HSE::Rotation& rotation) {
	vec3 input_dir = vec3(0,0,0);
	if ( IsKeyDown(KEY_W) ) input_dir.x += 1.0;
	if ( IsKeyDown(KEY_S) ) input_dir.x -= 1.0;
	if ( IsKeyDown(KEY_A) ) input_dir.y += 1.0;
	if ( IsKeyDown(KEY_D) ) input_dir.y -= 1.0;

	input_dir = quat(rotation) * input_dir;
	input_dir.z = 0.0;
	float input_mag = length(input_dir);

	if (input_mag != 0) input_dir = normalize(input_dir);

	dir.value = input_dir;
}

void walking(HSE::Velocity& velocity, HSE::CharacterBody& body, const Walk& walk, MoveDir& dir) {
	if ( body.on_floor() ) velocity.z = 0.0;
	else velocity = vec3(velocity) + Game.get<PhysicsEngine>().get_gravity() * GetFrameTime();

	float a;
	if ( dir.value != vec3(0,0,0) )
		a = body.on_floor()? walk.acceleration : walk.acceleration_air;
	else
		a = body.on_floor()? walk.decceleration : walk.decceleration_air;

	auto wish_vel = dir.value * walk.max_speed;
	vec3 v = mix(vec3(velocity), wish_vel, GetFrameTime() * a);

	velocity.x = v.x;
	velocity.y = v.y;
}

void player_jump(Player player, HSE::CharacterBody& body, HSE::Velocity& velocity, const Jump& jump) {
	if ( !IsKeyPressed(KEY_SPACE) or !body.on_floor() ) return;

	velocity.z = jump.speed;
}

void player_fire(Player player, HeldWeapon& weapon) {
	if ( not IsMouseButtonDown(0) ) return;
	if ( weapon.entity.get<Timer>().active ) return;
	fire_weapon(weapon.entity);
}
