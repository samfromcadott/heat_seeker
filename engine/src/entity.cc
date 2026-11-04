#include <heat_seeker.hh>

void HSE::follow_parent(HSE::Entity e, const HSE::FollowParent& f, HSE::Position& p, HSE::Rotation& r) {
	p = quat(get<Rotation>( e.parent() )) * f.position;
	p = vec3(get<Position>( e.parent() )) + vec3(p);
	r = quat(get<Rotation>( e.parent() )) * f.rotation;
}
