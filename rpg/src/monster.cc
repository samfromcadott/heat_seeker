#include <heat_seeker.hh>

#include "components.hh"
#include "systems.hh"
#include "weapon.hh"

using namespace HSE;

void chase_target(flecs::entity monster, HSE::Position& p, HSE::Rotation& r, MoveDir& md, Target& t) {
	if ( not t.entity.is_valid() ) return;

	// Don't chase if already attacking
	if ( auto arsenal = get_if<Arsenal>(monster) ) {
		if ( get<Weapon>( arsenal->equipped() ).active )
			return;
	}

	vec3 target_pos = get<Position>(t.entity);
	vec3 pos = p;

	// Get direction to target
	vec3 dir = target_pos - pos;
	dir = normalize(dir);
	md.value = dir;

	// Rotate to movement direction
	float yaw = atan2(dir.y, dir.x);
	r = quat( vec3(0, 0, yaw) );

	// Aim the muzzle
	float horz_dist = distance( vec3(target_pos.x, target_pos.y, 0), vec3(pos.x, pos.y, 0) );
	float vert_dist = abs( target_pos.z-pos.z );
	float pitch = atan2(vert_dist, horz_dist);
	get<Muzzle>(monster).pitch = -pitch;
}

void find_target(flecs::entity monster, Target& target) {
	// If the target is invalid
		// Do a shape cast
		// If an enemy is in it make them the target

	auto player = Game.lookup("player");
	if ( not player.is_valid() ) return;

	// Do a hitscan to check for line of sight
	vec3 start = vec3(get<Position>(monster)) + vec3(0,0,1);
	vec3 end = vec3(get<Position>(player)) + vec3(0,0,1);

	auto hit = Game.get<PhysicsEngine>().ray_cast(start, end - start, Layers::NON_MOVING);

	// If it hits a wall them make target null
	if (not hit.hit) {
		target.entity = player;
	}

	else {
		target.entity = Game.entity(0);
		if ( auto dir = get_if<MoveDir>(monster) )
			dir->value = vec3(0,0,0);
	}
}

void choose_attack(flecs::entity monster, HSE::Position& p, Target& target, Arsenal& arsenal, const ArsenalInfo& info) {
	if ( not target.entity.is_valid() ) return;
	if ( get<Weapon>( arsenal.equipped() ).active ) return;

	float dist = distance( vec3(p), vec3( get<Position>(target.entity) ) );

	// Make a random weighted choice
	int weight_sum = 0;
	for (auto n : info.weight) weight_sum += n;

	int r = rand() % (weight_sum + 1);
	for (int i = 0; i < arsenal.weapons.size(); i++) {
		r -= info.weight[i];

		if ( r > info.weight[i] ) continue;
		if ( dist < info.min[i] ) continue;
		if ( dist > info.max[i] ) continue;

		arsenal.index = i;
		fire_weapon( arsenal.equipped() );

		if ( auto anim = get_if<WeaponAnim>( arsenal.equipped() ) )
			get<HSE::Model>(monster).play( anim->fire );

		if ( auto move_dir = get_if<MoveDir>(monster) )
			move_dir->value = vec3(0,0,0);

		break;
	}
}

void monster_animation(flecs::entity monster, Arsenal& arsenal, HSE::Model& m) {
	vec3 v = get<Velocity>(monster);
	if (not get<Weapon>( arsenal.equipped() ).active) {
		if ( length(v) < 0.1 )
			m.play("Idle");
		else
			m.play("Walk");
	}

}
