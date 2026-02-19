#include <heat_seeker.hh>

#include "components.hh"
#include "systems.hh"
#include "weapon.hh"

flecs::world HSE::Game;
std::map<std::string, HSE::ModelData> HSE::model_files;
Camera3D HSE::camera = {
	.position = Vector3 { 0.0f, 10.0f, 10.0f },
	.target = Vector3 { 0.0f, 0.0f, 0.0f },
	.up = Vector3 { 0.0f, 0.0f, 1.0f },
	.fovy = 45.0f,
	.projection = CAMERA_PERSPECTIVE,
};
Shader HSE::gouraud_shader;
Texture2D HSE::uv_debug_texture;

using namespace HSE;

int main() {
	HSE::init("R.P.G. Game", 1280, 720);
	DisableCursor();

	uv_debug_texture = LoadTexture("base/textures/uv_debug.png");

	Game.import<HseCore>();

	Game.system<PlayerCamera&, Position&, Rotation&>().each(mouse_look);
	Game.system<Player, Velocity&, GroundMovement&, Rotation&>().each(player_movement);
	Game.system<HSE::Position&, HSE::Velocity&, HSE::CharacterBody&, GroundMovement&>().each(ground_movement);
	Game.system<Player, HSE::CharacterBody&, HSE::Velocity&, const Jump&>().each(player_jump);
	Game.system<Player, HeldWeapon&>().each(player_fire);
	Game.system<Weapon&, Timer&>().each(weapon_update);
	Game.system<Weapon&, Timer&, LaunchMissile&>().each(launch_missile);
	Game.system<Weapon&, Timer&, Hitscan&, Damage&>().each(launch_hitscan);
	Game.system<Health&>().each(die_when_no_health);
	Game.system<Position&, GroundMovement&, Target&>().each(chase_target);

	// Register components
	Game.component<Player>();
	Game.component<Monster>();
	Game.component<Missile>();

	Game.component<Health>()
		.member("max", &Health::max)
		.member("now", &Health::now);

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

	Game.component<Jump>()
		.member("speed", &Jump::speed);

	Game.component<Timer>()
		.member("active", &Timer::active)
		.member("time", &Timer::time);

	Game.component<Weapon>()
		.member("launch_time", &Weapon::launch_time)
		.member("rate", &Weapon::rate)
		.member("has_fired", &Weapon::has_fired);

	Game.component<HeldWeapon>()
		.member("entity", &HeldWeapon::entity);

	Game.component<Damage>()
		.member("value", &Damage::value);

	Game.component<Hitscan>()
		.member("range", &Hitscan::range);

	Game.component<LaunchMissile>()
		.member("missile", &LaunchMissile::missile)
		.member("speed", &LaunchMissile::speed);

	// Observers
	Game.observer<Target>()
	.event(flecs::OnAdd)
	.with<Monster>()
	.each([&](flecs::entity entity, Target& t) {
		t.entity = Game.lookup("player");
	});

	Game.observer<ContactAdded>()
	.event(flecs::OnSet)
	.with<Missile>()
	.each([](flecs::entity entity, ContactAdded& contact) {
		// Check if the entity hit has health
		if ( contact.other.has<Health>() and entity.has<Damage>() )
			contact.other.get_mut<Health>().now -= entity.get<Damage>().value;

		entity.destruct();
	});

	Game.observer<HeldWeapon>()
	.event(flecs::OnSet)
	.each([&](flecs::entity owner, HeldWeapon& hw) {
		if ( !hw.entity.is_valid() ) return;
		hw.entity.child_of(owner);
	});

	// Load scripts
	Game.script().filename("base/script/player.flecs").run();
	Game.script().filename("base/script/can.flecs").run();
	Game.script().filename("base/script/scene.flecs").run();
	Game.script().filename("base/script/ball.flecs").run();
	Game.script().filename("base/script/zombie.flecs").run();

	Game.import<flecs::stats>();

	// Creates REST server on default port (27750)
	Game.set<flecs::Rest>({});

	// Setup the HUD
	ui_function = [&]() {
		DrawFPS(10, 10);
		DrawCircle(1280/2, 720/2, 2.0, WHITE);
		auto p = Game.lookup("player");

		DrawText("HEALTH", 10, 680, 10, RED);

		if ( !p.is_valid() or !p.is_alive() ) return;

		int health = p.get<Health>().now;
		DrawText(TextFormat("%d", health), 10, 690, 20, RED);
	};

	// Main game loop
	while ( !WindowShouldClose() ) {
		Game.progress();
	}

	HSE::quit();
	UnloadTexture(uv_debug_texture);

	return 0;
}
