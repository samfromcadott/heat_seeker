#pragma once

#include <heat_seeker.hh>
#include "components.hh"

void fire_weapon(flecs::entity weapon);
void reset_weapon(HSE::Entity weapon);
void launch_missile(flecs::entity entity);
void launch_hitscan(flecs::entity entity);
void explode(HSE::Entity entity);
void missile_rotate(const HSE::Velocity& velocity, HSE::Rotation& rotation);
