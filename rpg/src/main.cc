#include <heat_seeker.hh>

#include "components.hh"
#include "systems.hh"

flecs::world HSE::Game;
std::map<std::string, HSE::ModelData> HSE::model_files;
Camera3D HSE::camera = {
	.position = Vector3 { 0.0f, 10.0f, 10.0f },
	.target = Vector3 { 0.0f, 0.0f, 0.0f },
	.up = Vector3 { 0.0f, 0.0f, 1.0f },
	.fovy = 45.0f,
	.projection = CAMERA_PERSPECTIVE,
};

using namespace HSE;

int main() {
	const int screenWidth = 1280;
	const int screenHeight = 720;

	InitWindow(screenWidth, screenHeight, "HEAT SEEKER");

	SetTargetFPS(60);
	DisableCursor();

	Game.import<HseCore>();

	Game.system<PlayerCamera&, Position&, Rotation&>().each(mouse_look);
	Game.system<Player, Velocity&, GroundMovement&, Rotation&>().each(player_movement);
	Game.system<HSE::Position&, HSE::Velocity&, HSE::CharacterBody&, GroundMovement&>().each(ground_movement);
	Game.system<Player, HSE::CharacterBody&, HSE::Velocity&>().each(player_jump);
	Game.system<Player, HSE::Position&, HSE::Rotation&>().each(shoot_ball);

	Game.component<Player>();

	Game.component<GroundMovement>()
		.member("direction", &GroundMovement::direction)
		.member("max_speed", &GroundMovement::max_speed)
		.member("acceleration", &GroundMovement::acceleration)
		.member("decceleration", &GroundMovement::decceleration)
		.member("acceleration_air", &GroundMovement::acceleration_air)
		.member("decceleration_air", &GroundMovement::decceleration_air);

	Game.component<PlayerCamera>()
		.member("rotation", &PlayerCamera::rotation)
		.member("offset", &PlayerCamera::offset);

	Game.script().filename("base/script/player.flecs").run();
	Game.script().filename("base/script/can.flecs").run();
	Game.script().filename("base/script/scene.flecs").run();
	Game.script().filename("base/script/ball.flecs").run();

	Game.import<flecs::stats>();

	// Creates REST server on default port (27750)
	Game.set<flecs::Rest>({});

	// Main game loop
	while ( !WindowShouldClose() ) {
		Game.progress();
	}

	CloseWindow();

	return 0;
}
