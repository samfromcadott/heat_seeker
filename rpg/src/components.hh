#pragma once

#include <heat_seeker.hh>

struct Player {}; // Tag for player entity

struct PlayerCamera {
	HSE::vec3 rotation;
	HSE::vec3 offset;
};

struct GroundMovement {
	HSE::vec3 direction;
	float max_speed = 10.0;
	float acceleration = 3.125;
	float decceleration = 3.125;
	float acceleration_air = 0.5;
	float decceleration_air = 0.0;
};
