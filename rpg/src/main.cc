#include <heat_seeker.hh>

#include "components.hh"
#include "systems.hh"
#include "weapon.hh"

flecs::world HSE::Game;
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

	init_core(Game);
	init_physics(Game);
	init_render(Game);
	File::mount("base");

	// Create systems
	Game.system<PlayerCamera&, Position&, Rotation&>("mouse_look").each(mouse_look);
	Game.system<Player, Velocity&, MoveDir&, Rotation&>("player_movement").each(player_movement);
	Game.system<HSE::Velocity&, HSE::CharacterBody&, const Walk&, MoveDir&>("walking").each(walking);
	Game.system<Player, HSE::CharacterBody&, HSE::Velocity&, const Jump&>("player_jump").each(player_jump);
	Game.system<Player, HeldWeapon&>("player_fire").each(player_fire);
	Game.system<Weapon&, Timer&>("weapon_update").each(weapon_update);
	Game.system<Weapon&, Timer&, LaunchMissile&>("launch_missile").each(launch_missile);
	Game.system<Weapon&, Timer&, Hitscan&, Damage&>("launch_hitscan").each(launch_hitscan);
	Game.system<Health&>("die_when_no_health").each(die_when_no_health);
	Game.system<Position&, Rotation&, MoveDir&, Target&>("chase_target").each(chase_target);
	Game.system<Position&, Target&, MeleeAttack&>("melee_attack").each(melee_attack);
	Game.system<HSE::Model&>("monster_animation").with<Monster>().each(monster_animation);

	// Register components
	Game.component<Player>();
	Game.component<Monster>();
	Game.component<Missile>();

	Game.component<Health>()
		.member("max", &Health::max)
		.member("now", &Health::now);

	Game.component<MoveDir>()
		.member("value", &MoveDir::value);

	Game.component<Walk>()
		.member("max_speed", &Walk::max_speed)
		.member("acceleration", &Walk::acceleration)
		.member("decceleration", &Walk::decceleration)
		.member("acceleration_air", &Walk::acceleration_air)
		.member("decceleration_air", &Walk::decceleration_air);

	Game.component<PlayerCamera>()
		.member("pitch", &PlayerCamera::pitch)
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

	Game.component<MeleeAttack>()
		.member("weapon", &MeleeAttack::weapon)
		.member("range", &MeleeAttack::range);

	Game.component<WeaponSound>()
		.member("fire", &WeaponSound::fire);

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
		if (entity.has<Owner>() and entity.get<Owner>().entity == contact.other)
			return;

		// Check if the entity hit has health
		if ( contact.other.has<Health>() and entity.has<Damage>() )
			contact.other.get_mut<Health>().now -= entity.get<Damage>().value;

		entity.destruct();
	});

	Game.observer<HeldWeapon>()
	.event(flecs::OnSet)
	.each([&](flecs::entity owner, HeldWeapon& hw) {
		if ( !hw.entity.is_valid() ) return;
		if ( hw.entity.has(flecs::Prefab) )
			hw.entity = Game.entity().is_a(hw.entity);

		hw.entity.child_of(owner);
	});

	Game.observer<MeleeAttack>()
	.event(flecs::OnSet)
	.each([&](flecs::entity owner, MeleeAttack& ma) {
		if ( !ma.weapon.is_valid() ) return;
		if ( ma.weapon.has(flecs::Prefab) )
			ma.weapon = Game.entity().is_a(ma.weapon);

		ma.weapon.child_of(owner);
	});

	Game.observer<WeaponSound>()
	.event(flecs::OnSet)
	.each([&](flecs::entity owner, WeaponSound& ws) {
		ws.fire.load();
	});

	// Load scripts
	Game.script().filename("base/script/player.flecs").run();
	Game.script().filename("base/script/can.flecs").run();
	Game.script().filename("base/script/zombie.flecs").run();

	// Load the first map
	load_level(Game, "base/maps/test.hsm");

	Game.import<flecs::stats>();

	// Creates REST server on default port (27750)
	Game.set<flecs::Rest>({});

	// Setup the HUD
	ui_function = [&]() {
		DrawFPS(10, 10);
		DrawCircle(1280/2, 720/2, 2.0, GREEN);
		auto p = Game.lookup("player");

		DrawText("HEALTH", 10, 680, 10, GREEN);
		DrawText("SPEED", 1200, 680, 10, GREEN);

		if ( !p.is_valid() or !p.is_alive() ) return;

		int health = p.get<Health>().now;
		DrawText(TextFormat("%d", health), 10, 690, 20, GREEN);

		float speed = length( vec3(p.get<Velocity>()) );
		DrawText(TextFormat("%02.02f", speed), 1200, 690, 20, GREEN);
	};

	// Main game loop
	while ( !WindowShouldClose() ) {
		Game.progress();
	}

	// auto room = Game.entity("room");
	// room.destruct();
	Game.remove_all<HSE::Model>();

	HSE::quit();
	UnloadTexture(uv_debug_texture);

	return 0;
}
