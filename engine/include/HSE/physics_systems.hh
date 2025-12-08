#pragma once

namespace HSE {

void pos_to_body(Body& b, Position& p);
void rot_to_body(Body& b, Rotation& r);
void vel_to_body(Body& b, Velocity& r);
void physics_update(PhysicsEngine& pe);
void body_to_pos(Body& b, Position& p);
void body_to_rot(Body& b, Rotation& r);
void body_to_vel(Body& b, Velocity& r);

}
