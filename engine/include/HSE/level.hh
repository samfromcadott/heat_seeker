#pragma once

#include <flecs.h>

namespace HSE {

typedef std::map<std::string, std::string> EntityDict;

void load_level(flecs::world& world, const std::string& filename);
flecs::entity parse_entity(flecs::world& world, const std::string& name, const nlohmann::json& json);
void parse_component(flecs::entity& entity, const std::string& component, const nlohmann::json& json);
void add_level_model(flecs::entity& entity, const nlohmann::json& json);
void add_level_collider(flecs::entity& entity);
Material load_level_material(const std::string& name);

}
