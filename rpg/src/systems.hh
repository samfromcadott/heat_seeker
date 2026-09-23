#pragma once

#include <heat_seeker.hh>

void mouse_look(PlayerCamera& player_camera, HSE::Position& position, HSE::Rotation& rotation);
void dead_camera(PlayerCamera& player_camera, HSE::Position& position, HSE::Rotation& rotation);
void player_movement(Player player, HSE::Velocity& velocity, MoveDir& dir, HSE::Rotation& rotation);
void walking(HSE::Velocity& velocity, HSE::CharacterBody& body, const Walk& walk, MoveDir& dir);
void player_jump(Player player, HSE::CharacterBody& body, HSE::Velocity& velocity, const Jump& jump);
// void player_fire(Player player, HeldWeapon& weapon);
void player_fire(Arsenal& arsenal);
void switch_weapon(Arsenal& arsenal);
void die_when_no_health(flecs::entity e, Health& h);
void chase_target(flecs::entity monster, HSE::Position& p, HSE::Rotation& r, MoveDir& md, Target& t);
void choose_attack(flecs::entity monster, HSE::Position& p, Target& target, Arsenal& arsenal, const ArsenalInfo& info);
void monster_animation(Arsenal& arsenal, HSE::Model& m);
