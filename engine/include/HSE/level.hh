#pragma once

#include <flecs.h>

namespace HSE {

typedef std::map<std::string, std::string> EntityDict;

void load_level(flecs::world& world, const std::string& filename);
flecs::entity parse_entity(flecs::world& world, const EntityDict& dict);
flecs::entity parse_entity(flecs::world& world, const nlohmann::json& json);
void parse_component(flecs::entity& entity, const std::string& component, const std::string& value);
void parse_component(flecs::entity& entity, const std::string& component, const nlohmann::json& json);
std::vector<std::string> split_value(const std::string& value);
void add_level_model(flecs::entity& entity, const nlohmann::json& json);
void add_level_collider(flecs::entity& entity);

}
