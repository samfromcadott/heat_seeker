#pragma once

#include <flecs.h>

namespace HSE {

typedef std::map<std::string, std::string> EntityDict;

void load_level(flecs::world& world, const std::string& filename);
flecs::entity parse_entity(flecs::world& world, const EntityDict& dict);
void parse_component(flecs::entity& entity, const std::string& component, const std::string& value);
std::vector<std::string> split_value(const std::string& value);

}
