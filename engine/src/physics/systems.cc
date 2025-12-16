#include <heat_seeker.hh>

using namespace HSE;

void HSE::pos_to_body(Body& b, Position& p) {
	if ( b.get_position() != glm::vec3(p) )
		b.set_position(p);
}

void HSE::rot_to_body(Body& b, Rotation& r) {
	if ( b.get_rotation() != glm::quat(r) )
		b.set_rotation(r);
}

void HSE::vel_to_body(Body& b, Velocity& v) {
	if ( b.get_velocity() != glm::vec3(v) )
		b.set_velocity(v);
}

void HSE::pos_to_character(CharacterBody& b, Position& p) {
	if ( b.get_position() != glm::vec3(p) )
		b.set_position(p);
}

void HSE::rot_to_character(CharacterBody& b, Rotation& r) {
	if ( b.get_rotation() != glm::quat(r) )
		b.set_rotation(r);
}

void HSE::vel_to_character(CharacterBody& b, Velocity& v) {
	if ( b.get_velocity() != glm::vec3(v) )
		b.set_velocity(v);
}

void HSE::physics_update(PhysicsEngine& pe) {
	pe.update();
}

void HSE::character_update(CharacterBody& b) {
	b.update();
}

void HSE::body_to_pos(Body& b, Position& p) {
	p = b.get_position();
}

void HSE::body_to_rot(Body& b, Rotation& r) {
	r = b.get_rotation();
}

void HSE::body_to_vel(Body& b, Velocity& v) {
	v = b.get_velocity();
}

void HSE::character_to_pos(CharacterBody& b, Position& p) {
	p = b.get_position();
}

void HSE::character_to_rot(CharacterBody& b, Rotation& r) {
	r = b.get_rotation();
}

void HSE::character_to_vel(CharacterBody& b, Velocity& v) {
	v = b.get_velocity();
}
