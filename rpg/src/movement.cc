#include <heat_seeker.hh>

#include "components.hh"
#include "systems.hh"

using namespace HSE;

void walking(HSE::Velocity& velocity, HSE::CharacterBody& body, const Walk& walk, MoveDir& dir) {
	vec3 ground_vel = body.get_ground_velocity();

	if ( body.on_floor() ) velocity.z = ground_vel.z;
	else velocity = vec3(velocity) + Game.get<PhysicsEngine>().get_gravity() * GetFrameTime();

	float a;
	if ( dir.value != vec3(0,0,0) )
		a = body.on_floor()? walk.acceleration : walk.acceleration_air;
	else
		a = body.on_floor()? walk.decceleration : walk.decceleration_air;

	auto wish_vel = dir.value * walk.max_speed;
	vec3 v = move_toward(vec3(velocity), wish_vel + ground_vel, GetFrameTime() * a);

	velocity.x = v.x;
	velocity.y = v.y;
}
