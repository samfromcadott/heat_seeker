#include <heat_seeker.hh>

using namespace std;

void HSE::load_prefab_file(flecs::world& world, const std::string& filename) {
	std::cout << "Loading prefab file " << filename << '\n';
	std::ifstream map_file(filename); // Load map
	auto json_file = nlohmann::json::parse(map_file); // Convert to JSON object

	// Loop through entities in ENT
	for ( const auto& [name, e] : json_file["ENT"].items() ) {
		flecs::entity new_prefab;

		// Make entity from prefab
		if ( e.contains("INHERIT") ) {
			string prefab_name = e["INHERIT"];
			auto prefab = world.lookup( prefab_name.c_str() );
			new_prefab = world.prefab().is_a(prefab);
		}
		else {
			new_prefab = world.prefab();
		}

		new_prefab.set_name( name.c_str() );
	}
	for ( const auto& [name, e] : json_file["ENT"].items() ) {
		std::cout << "Parsing prefab " << name << '\n';
		parse_prefab(world, name, e);
	}
}

flecs::entity HSE::parse_prefab(flecs::world& world, const std::string& name, const nlohmann::json& json) {
	flecs::entity new_prefab = world.lookup( name.c_str() );
 //
	// // Make entity from prefab
	// if ( json.contains("INHERIT") ) {
	// 	string prefab_name = json["INHERIT"];
	// 	auto prefab = world.lookup( prefab_name.c_str() );
	// 	new_prefab = world.prefab().is_a(prefab);
	// }
	// else {
	// 	new_prefab = world.prefab();
	// }
 //
	// new_prefab.set_name( name.c_str() );

	// Add components
	for ( const auto& [key, value] : json["COMP"].items() ) {
		std::cout << "Parsing component " << key << '\n';
		parse_component(new_prefab, key, value);
	}

	// Add children if present

	return new_prefab;
}
