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
