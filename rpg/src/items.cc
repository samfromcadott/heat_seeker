#include "components.hh"
#include "events.hh"

using namespace HSE;

void touch_ammo_item(flecs::entity entity, ContactAdded& contact) {
	if ( not contact.other.has<Arsenal>() ) return;

	auto arsenal = contact.other.get_mut<Arsenal>();
	auto give_ammo = entity.get<GiveAmmo>();

	for (auto& weapon : arsenal.weapons) {
		if ( not weapon.has<Ammo>() ) continue;
		if ( not weapon.is_a(give_ammo.weapon) ) continue;

		weapon.get_mut<Ammo>().count += give_ammo.count;
		entity.destruct();
	}
}

void touch_weapon_item(flecs::entity entity, HSE::ContactAdded& contact) {
	if ( not contact.other.has<Arsenal>() ) return;

	auto& arsenal = contact.other.get_mut<Arsenal>();
	auto& give_weapon = entity.get<GiveWeapon>();
	auto world = entity.world();

	if ( arsenal.weapons.size() > give_weapon.slot and arsenal.weapons[give_weapon.slot].is_valid() )
		return;

	if (arsenal.weapons.size() <= give_weapon.slot)
		arsenal.weapons.resize(give_weapon.slot+1);

	arsenal.weapons[give_weapon.slot] = world.entity();
	arsenal.weapons[give_weapon.slot].is_a(give_weapon.weapon);
	arsenal.weapons[give_weapon.slot].child_of(contact.other);
	arsenal.index = give_weapon.slot;
	entity.destruct();
}
