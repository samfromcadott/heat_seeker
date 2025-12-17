#pragma once

void mouse_look(PlayerCamera& player_camera, HSE::Position& position, HSE::Rotation& rotation);
void player_movement(Player player, HSE::Position& position, HSE::Rotation& rotation, HSE::Velocity& velocity, HSE::CharacterBody& body, GroundMovement& movement);
void player_jump(Player player, HSE::CharacterBody& body, HSE::Velocity& velocity);
void shoot_ball(Player player, HSE::Position& position, HSE::Rotation& rotation);
