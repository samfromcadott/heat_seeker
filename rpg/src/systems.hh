#pragma once

void mouse_look(PlayerCamera& player_camera, HSE::Position& position, HSE::Rotation& rotation);
void player_movement(Player player, HSE::Velocity& velocity, GroundMovement& movement, HSE::Rotation& rotation);
void ground_movement(HSE::Position& position, HSE::Velocity& velocity, HSE::CharacterBody& body, GroundMovement& movement);
void player_jump(Player player, HSE::CharacterBody& body, HSE::Velocity& velocity, const Jump& jump);
// void shoot_ball(Player player, HSE::Position& position, HSE::Rotation& rotation);
void player_fire(Player player, HeldWeapon& weapon);
void die_when_no_health(flecs::entity e, Health& h);
