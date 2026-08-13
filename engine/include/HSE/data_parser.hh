#pragma once

namespace HSE {

void load_prefab_file(flecs::world& world, const std::string& filename);
flecs::entity parse_prefab(flecs::world& world, const std::string& name, const nlohmann::json& json);
void parse_component(flecs::entity& entity, const std::string& component, const nlohmann::json& json);
void parse_component_member(flecs::world& world, const nlohmann::json& json, flecs::cursor& cur);
void parse_primitive(flecs::world& world, flecs::cursor& cur, const nlohmann::json& json);
void parse_struct(flecs::world& world, flecs::cursor& cur, const nlohmann::json& json);
void parse_enum(flecs::world& world, flecs::cursor& cur, const nlohmann::json& json);
void parse_custom(flecs::world& world, flecs::cursor& cur, const nlohmann::json& json);

}
