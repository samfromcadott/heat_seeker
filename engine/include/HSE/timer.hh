#pragma once

#include <stdint.h>
#include <functional>
#include <map>
#include <vector>
#include <list>
#include <string>

#include "entity.hh"

namespace HSE {

struct Timer {
	int time = 0;
	int entity = 0;
	int fn = -1;
};

typedef std::function< void(Entity) > TimerCallback;
typedef std::list<Timer> TimerList;

void update_timers(flecs::world world, TimerList& list);
void add_timer_callback(const std::string& name, TimerCallback fn);
void start_timer(Entity entity, const std::string& name, float time);

extern std::vector<TimerCallback> timer_callbacks;
extern std::map<std::string, int> timer_callback_id;

}
