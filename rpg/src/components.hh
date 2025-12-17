#pragma once

#include <heat_seeker.hh>

struct Player {}; // Tag for player entity

struct PlayerCamera {
	glm::vec3 rotation;
	glm::vec3 offset;
};

struct GroundMovement {
	float max_speed = 4.0;
	float acceleration = 3.0;
	float decceleration = 3.0;
	float acceleration_air = 0.5;
	float decceleration_air = 0.0;
};
