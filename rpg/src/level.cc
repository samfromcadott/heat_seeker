#include "level.hh"

using namespace HSE;

void use_parent(HSE::Entity entity, HSE::ContactAdded contact) {
	auto parent = entity.parent();

	if ( auto ds = get_if<DoorSliding>(parent) ) {
		if (ds->state == CLOSED) ds->state = OPENING;
	}
}

void slide_door(HSE::Entity entity, Position& position, Velocity& velocity, DoorSliding& ds) {
	if (ds.state == OPEN or ds.state == CLOSED) return;

	velocity = ds.state == OPENING? ds.speed * ds.direction : ds.speed * -ds.direction;

	vec3 goal = ds.state == OPENING? ds.start + ds.direction * ds.distance : ds.start;
	vec3 next_pos = vec3(position) + vec3(velocity) * GetFrameTime();

	// Stop if close to goal
	if ( distance(goal, next_pos) < 0.1 ) {
		ds.state = ds.state == OPENING? OPEN : CLOSED;
		position = goal;
		velocity = vec3(0,0,0);
	}
}
