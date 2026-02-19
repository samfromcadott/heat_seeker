#include <heat_seeker.hh>

#include "components.hh"
#include "systems.hh"
#include "weapon.hh"

using namespace HSE;

void chase_target(Position& p, GroundMovement& gm, Target& t) {
	if ( !t.entity.is_valid() or !t.entity.is_alive() ) return;

	// Get direction to target
	vec3 dir = vec3( t.entity.get<Position>() ) - vec3(p);
	dir = normalize(dir);
	gm.direction = dir;
}

void melee_attack(flecs::entity monster, HSE::Position& p, Target& t, MeleeAttack& attack) {
	// Check if the monster has a target
	if ( !t.entity.is_valid() or !t.entity.is_alive() ) return;

	// Check distance to target
	float dist = distance( vec3(p), vec3( t.entity.get<Position>() ) );
	if (dist < attack.range) fire_weapon(attack.weapon);

	// Stop monster if it's moving
	if ( monster.has<GroundMovement>() )
		monster.get_mut<GroundMovement>().direction = vec3(0,0,0);
}
