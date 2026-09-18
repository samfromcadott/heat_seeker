#pragma once

void touch_ammo_item(flecs::entity entity, HSE::ContactAdded& contact);
void touch_weapon_item(flecs::entity entity, HSE::ContactAdded& contact);
void player_die(HSE::Entity player);
