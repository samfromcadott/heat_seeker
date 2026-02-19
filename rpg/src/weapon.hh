#pragma once

#include <heat_seeker.hh>
#include "components.hh"

void weapon_update(Weapon& weapon, Timer& timer);
void fire_weapon(flecs::entity weapon);
void launch_missile(Weapon& weapon, Timer& timer, LaunchMissile& lm);
void launch_hitscan(flecs::entity entity, Weapon& weapon, Timer& timer, Hitscan& hs, Damage& d);
