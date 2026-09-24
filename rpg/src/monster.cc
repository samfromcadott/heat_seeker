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
		monster.get_mut<HSE::Model>().play("Attack");

		if ( auto move_dir = get_if<MoveDir>(monster) )
			move_dir->value = vec3(0,0,0);

		break;
	}
}

void monster_animation(Arsenal& arsenal, HSE::Model& m) {
	if (not arsenal.equipped().get<WeaponTimer>().active)
		m.play("Walk");
}
