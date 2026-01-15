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

struct Jump {
	float speed;
};

struct Timer {
	bool active;
	float timer; // Time since it became active
};

struct Weapon {
	float launch_time; // Time until the proectile/hitscan is fired
	float rate; // Time until it can fire again
};

struct Damage {
	int value;
};

struct Hitscan {
	float range;
};

struct LaunchMissile {
	flecs::entity missile;
	float speed;
};
