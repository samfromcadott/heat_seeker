#pragma once

#include <heat_seeker.hh>

void mouse_look(PlayerCamera& player_camera, HSE::Position& position, HSE::Rotation& rotation);
void player_movement(Player player, HSE::Velocity& velocity, MoveDir& dir, HSE::Rotation& rotation);
void walking(HSE::Velocity& velocity, HSE::CharacterBody& body, const Walk& walk, MoveDir& dir);
void player_jump(Player player, HSE::CharacterBody& body, HSE::Velocity& velocity, const Jump& jump);
void player_fire(Player player, HeldWeapon& weapon);
void die_when_no_health(flecs::entity e, Health& h);
void chase_target(HSE::Position& p, HSE::Rotation& r, MoveDir& md, Target& t);
void melee_attack(flecs::entity e, HSE::Position& p, Target& t, MeleeAttack& attack);
void monster_animation(flecs::entity e, HSE::Model& m);
