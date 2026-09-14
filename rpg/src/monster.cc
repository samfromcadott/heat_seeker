#include <heat_seeker.hh>

#include "components.hh"
#include "systems.hh"
#include "weapon.hh"

using namespace HSE;

void chase_target(HSE::Position& p, HSE::Rotation& r, MoveDir& md, Target& t) {
	if ( !t.entity.is_valid() or !t.entity.is_alive() ) return;

	// Get direction to target
	vec3 dir = vec3( t.entity.get<Position>() ) - vec3(p);
	dir = normalize(dir);
	md.value = dir;

	// Rotate to movement direction
	r = quat(vec3( 0, 0, atan2(dir.y, dir.x) ));
}

void melee_attack(flecs::entity monster, HSE::Position& p, Target& t, Arsenal& arsenal) {
	// Check if the monster has a target
	if ( !t.entity.is_valid() or !t.entity.is_alive() ) return;

	// Check distance to target
	float dist = distance( vec3(p), vec3( t.entity.get<Position>() ) );
	if (dist > 2.0) return;

	// Stop monster if it's moving
	if ( monster.has<MoveDir>() )
		monster.get_mut<MoveDir>().value = vec3(0,0,0);

	arsenal.index = 0;
	fire_weapon( arsenal.equipped() );
	monster.get_mut<HSE::Model>().play("Attack");
}

void monster_animation(Arsenal& arsenal, HSE::Model& m) {
	if (not arsenal.equipped().get<Timer>().active)
		m.play("Walk");
}
