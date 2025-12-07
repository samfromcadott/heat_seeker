#pragma once

namespace HSE {

void pos_to_body(Body& b, Position& p);
void rot_to_body(Body& b, Rotation& p);
void physics_update();
void body_to_pos(Body& b, Position& p);
void body_to_rot(Body& b, Rotation& p);

}
