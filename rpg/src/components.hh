#pragma once

#include <heat_seeker.hh>

struct Player {}; // Tag for player entity
struct Monster {};
struct Missile {};

struct Target {
	flecs::entity entity;
};

struct Owner {
	flecs::entity entity;
};

struct Health {
	int max = 100;
	int now = max;
};

struct PlayerCamera {
	float pitch = 0;
	HSE::vec3 offset;
};

struct MoveDir {
	HSE::vec3 value;
};

struct Walk {
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
	bool active = false;
	float time = 0.0; // Time since it became active
};

struct Weapon {
	float launch_time; // Time until the proectile/hitscan is fired
	float rate; // Time until it can fire again
	bool has_fired = true; // Has the weapon fired since the current cycle started
};

struct Muzzle {
	HSE::vec3 direction;
	HSE::vec3 offset;
};

// Current weapon used by a player
struct HeldWeapon {
	flecs::entity entity;
};

// Used by monsters to attack close enemies
struct MeleeAttack {
	flecs::entity weapon;
	float range;
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

struct WeaponSound {
	Sound sound;
};
