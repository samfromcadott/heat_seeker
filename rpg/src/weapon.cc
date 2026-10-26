#include <heat_seeker.hh>

#include "components.hh"
#include "systems.hh"
#include "weapon.hh"

using namespace HSE;

void fire_weapon(flecs::entity weapon) {
	auto& w = get<Weapon>(weapon);
	if (w.active) return; // If weapon is active do nothing

	start_timer(weapon, "reset_weapon", w.rate);
	w.active = true;

	// Launch immediatly is launch_time is 0
	if (w.launch_time <= 0.0) {
		if ( has<LaunchMissile>(weapon) ) launch_missile(weapon);
		if ( has<Hitscan>(weapon) ) launch_hitscan(weapon);
	}
	else {
		if ( has<LaunchMissile>(weapon) )
			start_timer(weapon, "launch_missile", w.launch_time);
		if ( has<Hitscan>(weapon) )
			start_timer(weapon, "launch_hitscan", w.launch_time);
	}

	// Use ammo
	if ( weapon.has<AmmoUse>() ) {
		auto& ammo = weapon.get_mut<Ammo>();
		auto& ammo_use = weapon.get<AmmoUse>();

		// Return if weapon doesn't have ammo
		if ( ammo.count <= 0 ) return;

		ammo.count -= ammo_use.cost;
	}

	// If the weapon has a sound play it
	if ( weapon.has<WeaponSound>() ) {
		auto& fire = weapon.get<WeaponSound>().fire;
		PlaySound(*fire);
	}
}

void reset_weapon(HSE::Entity weapon) {
	get<Weapon>(weapon).active = false;
}

void launch_missile(flecs::entity entity) {
	auto owner = entity.parent();

	vec3 launch_point, vel;

	// Set launch point
	vec3 pos = vec3( owner.get<Position>() );
	quat rot = quat( owner.get<Rotation>() );
	vec3 offset = get<Muzzle>(owner).offset;
	launch_point = pos + ( rot * offset );

	// Set velocity
	if ( owner.has<PlayerCamera>() ) {
		float yaw = eulerAngles( quat(rot) ).z;
		float pitch = owner.get<PlayerCamera>().pitch;

		vec3 dir = quat(vec3(0, pitch, yaw)) * vec3(1,0,0);
		vel = dir * get<LaunchMissile>(entity).speed;
	}
	else {
		vec3 dir = rot * vec3(1,0,0);
		vel = dir * get<LaunchMissile>(entity).speed;
	}

	flecs::entity missile = Game.entity().is_a( get<LaunchMissile>(entity).missile );

	missile.set<Position>( vec3(launch_point) );
	missile.set<Velocity>( vec3(vel) );

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
		start = p + vec3(0,0,1);
		dir = r * vec3(range, 0, 0);
	}

	// Check for collisions
	auto hit = Game.get<PhysicsEngine>().ray_cast(start, dir * range, Layers::MOVING);
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
	auto model = Game.entity();
	model.is_a( Game.lookup("rpg_explosion") );
	model.set<Position>( get<Position>(entity) );

	// Play the explosion sound
	if (get<Explode>(entity).sound)
		PlaySound( *get<Explode>(entity).sound );

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
			dir = normalize(dir);
			get<Body>(e).add_force(dir * force * coef);
		}
		else if ( has<CharacterBody>(e) ) {
			vec3 dir = get<CharacterBody>(e).get_center() - vec3(get<Position>(entity));
			dir = normalize(dir);
			get<Velocity>(e) = vec3(get<Velocity>(e)) + dir * force * coef * 0.001f;
		}

		if ( has<Health>(e) )
			get<Health>(e).now -= damage*coef;
	}

	entity.destruct();
}

void missile_rotate(const Velocity& velocity, Rotation& rotation) {
	vec3 v1 = vec3(1,0,0);
	vec3 v2 = normalize( vec3(velocity) );

	vec3 r = cross(v1, v2);

	rotation = normalize( quat(
		1.0f + dot(v1, v2),
		r.x, r.y, r.z
	) );
}
