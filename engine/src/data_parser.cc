#include <heat_seeker.hh>

using namespace std;
using namespace flecs::meta;
using namespace nlohmann;

void HSE::load_prefab_file(flecs::world& world, const std::string& filename) {
	std::cout << "Loading prefab file " << filename << '\n';
	auto map_file = File::open(filename);
	auto json_file = nlohmann::json::parse(map_file);

	// Loop through entities in ENT
	for ( const auto& [name, e] : json_file["ENT"].items() ) {
		flecs::entity new_prefab = world.prefab();
		new_prefab.set_name( name.c_str() );
	}

	// Parse prefabs
	for ( const auto& [name, e] : json_file["ENT"].items() ) {
		parse_prefab(world, name, e);
	}
}

flecs::entity HSE::parse_prefab(flecs::world& world, const std::string& name, const nlohmann::json& json) {
	flecs::entity new_prefab = world.lookup( name.c_str() );

	if ( json.contains("INHERIT") ) {
		string prefab_name = json["INHERIT"];
		auto prefab = world.lookup( prefab_name.c_str() );
		new_prefab.is_a(prefab);
	}

	// Add components
	for ( const auto& [key, value] : json["COMP"].items() ) {
		parse_component(new_prefab, key, value);
	}

	// Add children if present

	return new_prefab;
}

void HSE::parse_component(flecs::entity& entity, const string& component, const nlohmann::json& json) {
	// Get world
	flecs::world world = entity.world();

	// Get ID for component
	auto comp_ent = world.lookup( component.c_str() );
	if (comp_ent == 0) return; // Exit if component doesn't exist

	entity.add(comp_ent);
	if ( json.empty() ) return;
	void* ptr = entity.ensure(comp_ent);

	// Use flecs::cursor to go through component and set values
	flecs::cursor cur = world.cursor(comp_ent, ptr);
	cur.push();
	for ( const auto& [key, value] : json.items() ) {
		cur.member( key.c_str() );
		parse_component_member(world, value, cur);
	}
	cur.pop();
}

void HSE::parse_component_member(flecs::world& world, const nlohmann::json& json, flecs::cursor& cur) {
	auto type = cur.get_type().get<flecs::Type>().kind;
	switch (type) {
		case PrimitiveType: parse_primitive(world, cur, json); break;
		case StructType: parse_struct(world, cur, json); break;
		case EnumType: parse_enum(world, cur, json); break;
		case CustomType: parse_custom(world, cur, json); break;
		default: break;
	}
}

void HSE::parse_primitive(flecs::world& world, flecs::cursor& cur, const json& json) {
	auto kind = cur.get_type().get<flecs::Primitive>().kind;
	switch (kind) {
		case I8: cur.set_int(json); break;
		case I16: cur.set_int(json); break;
		case I32: cur.set_int(json); break;
		case I64: cur.set_int(json); break;
		case U8: cur.set_uint(json); break;
		case U16: cur.set_uint(json); break;
		case U32: cur.set_uint(json); break;
		case U64: cur.set_uint(json); break;
		case F32: cur.set_float(json); break;
		case F64: cur.set_float(json); break;
		case Bool: cur.set_bool(json); break;
		case Entity:
			if ( json.is_string() )
				cur.set_entity( world.lookup( string(json).c_str() ) );
			else if ( json.is_number_integer() )
				cur.set_entity( flecs::entity(world, json) );
			break;
		default: break;
	}
}

void HSE::parse_struct(flecs::world& world, flecs::cursor& cur, const json& json) {
	cur.push();
	for ( const auto& [key, value] : json.items() ) {
		cur.member( key.c_str() );
		parse_component_member(world, value, cur);
	}
	cur.pop();
}

void HSE::parse_enum(flecs::world& world, flecs::cursor& cur, const json& json) {
	cur.set_string( string(json).c_str() );
}

void HSE::parse_custom(flecs::world& world, flecs::cursor& cur, const nlohmann::json& json) {
	if ( json.is_string() )
		cur.set_string( string(json).c_str() );
}
