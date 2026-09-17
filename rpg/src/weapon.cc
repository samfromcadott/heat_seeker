#include <heat_seeker.hh>

#include "components.hh"
#include "systems.hh"
#include "weapon.hh"

using namespace HSE;

void weapon_update(flecs::entity entity, Weapon& weapon, WeaponTimer& timer) {
	if (!timer.active) return;

	timer.time += GetFrameTime();

	if (timer.time > weapon.launch_time and not weapon.has_fired) {
		if ( has<LaunchMissile>(entity) )
			launch_missile(entity);
		if ( has<Hitscan>(entity) )
			launch_hitscan(entity);

		weapon.has_fired = true;
	}

	// Reset the weapon at the end of its cycle
	if (timer.time > weapon.rate) {
		weapon.has_fired = false;
		timer.active = false;
		timer.time = 0.0;
	}
}

void fire_weapon(flecs::entity weapon) {
	if ( weapon.has<AmmoUse>() ) {
		auto& ammo = weapon.get_mut<Ammo>();
		auto& ammo_use = weapon.get<AmmoUse>();

		// Return if weapon doesn't have ammo
		if ( ammo.count <= 0 ) return;

		ammo.count -= ammo_use.cost;
	}

	auto& timer = weapon.get_mut<WeaponTimer>();
	if (not timer.active) {
		timer.active = true;
		get<Weapon>(weapon).has_fired = false;
	}

	// If the weapon has a sound play it
	if ( weapon.has<WeaponSound>() ) {
		auto& fire = weapon.get<WeaponSound>().fire;
		PlaySound(*fire);
	}
}

void launch_missile(flecs::entity entity) {
	auto owner = entity.parent();

	vec3 launch_point, vel;
	if ( owner.has<PlayerCamera>() ) {
		vec3 p = vec3( owner.get<Position>() );
		quat r = quat( owner.get<Rotation>() );

		float yaw = eulerAngles( quat(r) ).z;
		float pitch = owner.get<PlayerCamera>().pitch;

		vec3 dir = quat(vec3(0, pitch, yaw)) * vec3(1,0,0);
		launch_point = p + owner.get<PlayerCamera>().offset + dir;
		vel = dir * get<LaunchMissile>(entity).speed;
	}
	else {
		vec3 p = vec3( owner.get<Position>() );
		quat r = quat( owner.get<Rotation>() );
		launch_point = p + ( r * vec3(0.251,0,0) );
		vec3 dir = r * vec3(1,0,0);
		vel = dir * get<LaunchMissile>(entity).speed;
	}

	flecs::entity missile = Game.entity().is_a( get<LaunchMissile>(entity).missile );

	missile.set<Position>( vec3(launch_point) );
	missile.set<Velocity>( vec3(vel) );
	// missile.set<Rotation>(rotation);

	missile.set<Owner>({ entity.parent() });
}

void launch_hitscan(flecs::entity entity) {
	auto owner = entity.parent();
	auto range = get<Hitscan>(entity).range;

	// Create a raycast
	vec3 start, dir;
	if ( owner.has<PlayerCamera>() ) {
		vec3 p = vec3( owner.get<Position>() );
		quat r = quat( owner.get<Rotation>() );

		float yaw = eulerAngles( quat(r) ).z;
		float pitch = owner.get<PlayerCamera>().pitch;

		dir = quat(vec3(0, pitch, yaw)) * vec3(1,0,0);
		start = p + owner.get<PlayerCamera>().offset + dir;
	}
	else {
		vec3 p = vec3( owner.get<Position>() );
		quat r = quat( owner.get<Rotation>() );
		start = p + ( r * vec3(0.251,0,0) );
		dir = vec3(range, 0, 0);
		dir = r * dir;
	}

	// Check for collisions
	auto hit = Game.get<PhysicsEngine>().ray_cast(start, dir * range);
	if (not hit.hit) return;

	for (auto& e : hit.entities) {
		if ( not e.has<Health>() ) continue;
		if (e == owner) continue;

		e.get_mut<Health>().now -= get<Damage>(entity).value;
		break;
	}
}

void explode(Entity entity) {
	// Spawn an explosion model

	float force = get<Explode>(entity).force;
	float radius = get<Explode>(entity).radius;
	float damage = get<Damage>(entity).value;

	// Do a shape cast
	auto& engine = entity.world().get_mut<PhysicsEngine>();

	ShapeOptions shape;
	shape.type = ShapeType::SPHERE;
	shape.radius = radius;

	auto entities = engine.shape_cast(get<Position>(entity), shape);

	// Apply force and damage
	for (auto& e : entities) {
		if (e == entity) continue; // Skip self

		float dist = distance( vec3(get<Position>(entity)), vec3(get<Position>(e)) );
		float coef = std::lerp(1.0, 0.0, dist/radius);
		coef = std::clamp(coef, 0.0f, 1.0f);

		if ( has<Body>(e) ) {
			vec3 dir = get<Body>(e).get_center() - vec3(get<Position>(entity));
			get<Body>(e).add_force(force * dir * coef);
		}
		else if ( has<CharacterBody>(e) ) {
			vec3 dir = get<CharacterBody>(e).get_center() - vec3(get<Position>(entity));
			get<Velocity>(e) = vec3(get<Velocity>(e)) + force * dir * coef * 0.05f;
		}

		if ( has<Health>(e) )
			get<Health>(e).now -= damage*coef;
	}

	entity.destruct();
}
