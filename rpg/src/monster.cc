#include <heat_seeker.hh>

#include "components.hh"
#include "systems.hh"
#include "weapon.hh"

using namespace HSE;

void chase_target(flecs::entity monster, HSE::Position& p, HSE::Rotation& r, MoveDir& md, Target& t) {
	if ( !t.entity.is_valid() or !t.entity.is_alive() ) return;
	if ( auto arsenal = get_if<Arsenal>(monster) ) {
		if ( get<WeaponTimer>( arsenal->equipped() ).active )
			return;
	}

	// Get direction to target
	vec3 dir = vec3( t.entity.get<Position>() ) - vec3(p);
	dir = normalize(dir);
	md.value = dir;

	// Rotate to movement direction
	float yaw = atan2(dir.y, dir.x);
	r = quat( vec3(0, 0, yaw) );
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
	if ( arsenal.equipped().get<WeaponTimer>().active ) return;

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
	if (not arsenal.equipped().get<WeaponTimer>().active) {
		if ( length(v) < 0.1 )
			m.play("Idle");
		else
			m.play("Walk");
	}

}
