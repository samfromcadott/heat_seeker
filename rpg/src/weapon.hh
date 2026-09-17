#pragma once

#include <heat_seeker.hh>
#include "components.hh"

void weapon_update(flecs::entity entity, Weapon& weapon, WeaponTimer& timer);
void fire_weapon(flecs::entity weapon);
void launch_missile(flecs::entity entity);
void launch_hitscan(flecs::entity entity);
void explode(HSE::Entity entity);
