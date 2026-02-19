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
	// missile.set<Rotation>(rotation);

	weapon.has_fired = true;
}

void launch_hitscan(flecs::entity entity, Weapon& weapon, Timer& timer, Hitscan& hs, Damage& d) {
	if (weapon.has_fired) return;
	if (timer.time < weapon.launch_time) return;

	// Get the weapon's owner
	auto owner = entity.parent();

	// Create a raycast
	vec3 p = vec3( owner.get<Position>() );
	quat r = quat( owner.get<Rotation>() );
	vec3 start = p + ( r * vec3(1,0,0) );
	// vec3 start = p + vec3(0,0,10);
	vec3 dir(hs.range, 0, 0);
	// vec3 dir(0, 0, -hs.range);
	dir = r * dir;

	// Check for collisions
	// std::cout << "CHECKING FOR HIT...\n";
	auto hit = Game.get<PhysicsEngine>().ray_cast(start, dir);
	if (!hit.hit) return;
	std::cout << "HAD HIT!\n";

	// Reduce health for intersecting entity with Health component
	if ( !hit.entity.has<Health>() ) return;
	if ( !hit.entity.is_valid() or !hit.entity.is_alive() ) return;
	hit.entity.get_mut<Health>().now -= d.value;

	weapon.has_fired = true;
}
