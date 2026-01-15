#include <heat_seeker.hh>

#include "components.hh"
#include "systems.hh"

using namespace HSE;

void weapon_update(Weapon& weapon, Timer& timer) {
	if (!timer.active) return;

	timer.time += GetFrameTime();

	if (timer.time < weapon.rate) return;

	// Reset the weapon at the end of its cycle
	weapon.has_fired = false;
	timer.active = false;
	timer.time = 0.0;
}

void fire_weapon(flecs::entity weapon) {
	auto& timer = weapon.get_mut<Timer>();
	if (!timer.active) timer.active = true;
}

void launch_missile(Weapon& weapon, Timer& timer, LaunchMissile& lm) {
	if (weapon.has_fired) return;
	if (timer.time < weapon.launch_time) return;

	vec3 offset = raylib_to_glm(camera.target) - raylib_to_glm(camera.position);
	vec3 launch_point = offset * 0.5f;
	launch_point += raylib_to_glm(camera.position);
	vec3 vel = offset * lm.speed;

	flecs::entity missile = Game.entity().is_a(lm.missile);

	missile.set<Position>( vec3(launch_point) );
	missile.set<Velocity>( vec3(vel) );
	// ball.set<Rotation>(rotation);

	weapon.has_fired = true;
}
