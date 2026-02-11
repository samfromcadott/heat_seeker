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
