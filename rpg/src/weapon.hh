#pragma once

#include <heat_seeker.hh>
#include "components.hh"

void weapon_update(Weapon& weapon, Timer& timer);
void fire_weapon(flecs::entity weapon);
void launch_missile(Weapon& weapon, Timer& timer, LaunchMissile& lm);
