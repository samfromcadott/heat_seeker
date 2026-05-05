#include <heat_seeker.hh>

using namespace HSE;
using namespace std;

void HSE::load_level(flecs::world& world, const string& filename) {
	// Make entities from models
	// Loop through entities in ENT
}

flecs::entity HSE::parse_entity(flecs::world& world, const EntityDict& dict) {
	// Make entity from prefab
	auto prefab = world.entity( dict.at("classname").c_str() );
	auto entity = world.entity().is_a(prefab);

	// Loop through keys
	for (const auto& [key, value] : dict) {
		if (key == "classname") continue;

		parse_component(entity, key, value);
	}

	return entity;
}

void HSE::parse_component(flecs::entity& entity, const string& component, const string& value) {
	// Get world
	flecs::world world = entity.world();

	// Get ID for component
	auto comp_ent = world.lookup( component.c_str() );
	if (comp_ent == 0) return; // Exit if component doesn't exist

	void* ptr = entity.ensure(comp_ent);

	// Split value by space
	auto members = split_value(value);

	// Use flecs::cursor to go through component and set values
	flecs::cursor cur = world.cursor(comp_ent, ptr);
	cur.push();
	for (const string& m : members) {
		string type = cur.get_type().name().c_str();

		if (type == "char") cur.set_char( m[0] );
		else if (type == "i32") cur.set_int( stoi(m) );
		else if (type == "i64") cur.set_int( stoi(m) );
		else if (type == "u32") cur.set_uint( stoi(m) );
		else if (type == "u64") cur.set_uint( stoi(m) );
		else if (type == "f32") cur.set_float( stof(m) );
		else if (type == "f64") cur.set_float( stof(m) );

		cur.next();
	}
	cur.pop();
}

vector<string> HSE::split_value(const string& value) {
	vector<string> result;
	stringstream stream(value);
	string item;

	while ( getline(stream, item, ' ') )
		result.push_back(item);

	return result;
}
