#include <heat_seeker.hh>

using namespace HSE;

std::vector<TimerCallback> HSE::timer_callbacks;
std::map<std::string, int> HSE::timer_callback_id;

void HSE::update_timers(flecs::world world, TimerList& list) {
	auto timer = list.begin();

	while ( timer != list.end() ) {
		Entity entity = flecs::entity(world, timer->entity);

		// Check if entity is valid
		if ( not entity.is_valid() ) {
			timer = list.erase(timer);
			continue;
		}

		// Update time
		timer->time -= GetFrameTime() * 1000;

		if (timer->time > 0) {
			++timer;
			continue;
		}

		// Execute callback
		timer_callbacks[timer->fn](entity);
		timer = list.erase(timer);
	}
}

void HSE::add_timer_callback(const std::string& name, TimerCallback fn) {
	timer_callbacks.push_back(fn);
	timer_callback_id[name] = timer_callbacks.size()-1;
}

void HSE::add_timer(Entity entity, const std::string& name, float time) {
	auto& timers = entity.world().get_mut<TimerList>();

	int t = time*1000;
	int id = entity.id();
	int fn = timer_callback_id[name];

	timers.push_back( {t, id, fn} );
}
