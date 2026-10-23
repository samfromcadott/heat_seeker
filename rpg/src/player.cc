#include <algorithm>
#include <heat_seeker.hh>

#include "components.hh"
#include "systems.hh"
#include "weapon.hh"
#include "movement.hh"

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
	camera.up = {0,0,1};
}

void dead_camera(PlayerCamera& player_camera, Position& position, Rotation& rotation) {
	auto camera_position = vec3(position) + player_camera.offset;

	vec3 camera_target = quat(rotation) * vec3(1,0,0);
	camera_target += camera_position;

	vec3 up = quat(rotation) * vec3(0,0,1);

	camera.target = glm_to_raylib(camera_target);
	camera.position = glm_to_raylib(camera_position);
	camera.up = glm_to_raylib(up);
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

void player_jump(Player player, HSE::CharacterBody& body, HSE::Velocity& velocity, const Jump& jump) {
	if ( IsKeyPressed(KEY_SPACE) and body.on_floor() )
		velocity.z += jump.speed;
}

void player_crouch(Entity player, HSE::CharacterBody& body, Crouch& c) {
	if ( IsKeyPressed(KEY_LEFT_CONTROL) )
		crouch(player);
	else if ( IsKeyReleased(KEY_LEFT_CONTROL) )
		uncrouch(player);
}

void player_fire(Arsenal& arsenal) {
	if ( not IsMouseButtonDown(0) ) return;
	if ( arsenal.equipped().get<WeaponTimer>().active ) return;
	fire_weapon( arsenal.equipped() );
}

void switch_weapon(Arsenal& arsenal) {
	const std::vector<int> keys = {
		KEY_ONE,
		KEY_TWO,
		KEY_THREE,
		KEY_FOUR,
		KEY_FIVE,
		KEY_SIX,
		KEY_SEVEN,
		KEY_EIGHT,
		KEY_NINE,
		KEY_ZERO
	};

	for (int i = 0; i < keys.size(); i++) {
		if ( i >= arsenal.weapons.size() ) break;
		if ( not IsKeyPressed(keys[i]) ) continue;
		if ( not arsenal.weapons[i].is_valid() ) break;

		arsenal.index = i;
		return;
	}
}

void player_die(Entity player) {
	auto world = player.world();
	auto head = world.entity().is_a( world.lookup("player_head") );

	vec3 p = get<Position>(player);
	quat r = get<Rotation>(player);
	auto camera = get<PlayerCamera>(player);

	head.set<Position>( p + camera.offset );

	float yaw = eulerAngles(r).z;
	float pitch = camera.pitch;
	head.set<Rotation>( quat(vec3(0, pitch, yaw)) );

	head.set<Velocity>( get<Velocity>(player) );
}
