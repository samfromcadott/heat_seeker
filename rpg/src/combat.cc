#include <heat_seeker.hh>

#include "components.hh"
#include "systems.hh"
#include "weapon.hh"
#include "events.hh"

using namespace HSE;

void die_when_no_health(flecs::entity e, Health& h) {
	if (h.now > 0) return;

	if ( has<Player>(e) ) player_die(e);
	else if ( has<Monster>(e) ) monster_die(e);
	else e.destruct();
}
