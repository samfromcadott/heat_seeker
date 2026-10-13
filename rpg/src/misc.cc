#include <heat_seeker.hh>

#include "components.hh"
#include "systems.hh"

void delete_timer(flecs::entity entity, DeleteTimer& timer) {
	timer.time -= GetFrameTime();
	if (timer.time <= 0) entity.destruct();
}
