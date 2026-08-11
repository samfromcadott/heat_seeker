#pragma once

namespace HSE {

void load_prefab_file(flecs::world& world, const std::string& filename);
flecs::entity parse_prefab(flecs::world& world, const std::string& name, const nlohmann::json& json);

}
