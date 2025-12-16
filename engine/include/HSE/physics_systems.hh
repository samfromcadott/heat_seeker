#pragma once

namespace HSE {

void pos_to_body(Body& b, Position& p);
void rot_to_body(Body& b, Rotation& r);
void vel_to_body(Body& b, Velocity& v);

void pos_to_character(CharacterBody& b, Position& p);
void rot_to_character(CharacterBody& b, Rotation& r);
void vel_to_character(CharacterBody& b, Velocity& v);

void physics_update(PhysicsEngine& pe);
void character_update(CharacterBody& b);

void body_to_pos(Body& b, Position& p);
void body_to_rot(Body& b, Rotation& r);
void body_to_vel(Body& b, Velocity& v);

void character_to_pos(CharacterBody& b, Position& p);
void character_to_rot(CharacterBody& b, Rotation& r);
void character_to_vel(CharacterBody& b, Velocity& v);

}
