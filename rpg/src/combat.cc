#include <heat_seeker.hh>

#include "components.hh"
#include "systems.hh"
#include "weapon.hh"


void die_when_no_health(flecs::entity e, Health& h) {
	if (h.now <= 0) e.destruct();
}
