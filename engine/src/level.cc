#include <heat_seeker.hh>

using namespace HSE;
using namespace std;
using namespace nlohmann;

void HSE::load_level(flecs::world& world, const string& filename) {
	std::ifstream map_file(filename); // Load map
	auto json_file = json::from_cbor(map_file); // Convert to JSON object

	// Loop through entities in ENT
	for (const auto& e : json_file["ENT"]) {
		flecs::entity entity = parse_entity(world, e);
	}

	// Make entities from models
	for (const auto& m : json_file["MODEL"]) {
		auto entity = world.entity();
		add_level_model(entity, m);
		add_level_collider(entity);
	}
}

flecs::entity HSE::parse_entity(flecs::world& world, const nlohmann::json& json) {
	flecs::entity entity;

	// Make entity from prefab
	if ( json.contains("INHERIT") ) {
		string prefab_name = json["INHERIT"];
		auto prefab = world.lookup( prefab_name.c_str() );
		entity = world.entity().is_a(prefab);
	}
	else {
		entity = world.entity();
	}

	// If it has a model convert and add it
	if ( json.contains("MODEL") ) {
		add_level_model(entity, json["MODEL"]);
		add_level_collider(entity);
	}

	// Loop through keys
	for ( const auto& [key, value] : json.items() ) {
		if (key == "INHERIT" or key == "MODEL") continue;

		parse_component(entity, key, value);
	}

	return entity;
}

void HSE::parse_component(flecs::entity& entity, const string& component, const nlohmann::json& json) {
	// Get world
	flecs::world world = entity.world();

	// Get ID for component
	auto comp_ent = world.lookup( component.c_str() );
	if (comp_ent == 0) return; // Exit if component doesn't exist

	void* ptr = entity.ensure(comp_ent);

	// Use flecs::cursor to go through component and set values
	flecs::cursor cur = world.cursor(comp_ent, ptr);
	cur.push();
	for ( const auto& [key, value] : json.items() ) {
		cur.member( key.c_str() );
		string type = cur.get_type().name().c_str();

		if (type == "i32") cur.set_int(value);
		else if (type == "i64") cur.set_int(value);
		else if (type == "u32") cur.set_uint(value);
		else if (type == "u64") cur.set_uint(value);
		else if (type == "f32") cur.set_float(value);
		else if (type == "f64") cur.set_float(value);
		else if (type == "string") cur.set_string( string(value).c_str() );

		cur.next();
	}
	cur.pop();
}

void HSE::add_level_model(flecs::entity& entity, const nlohmann::json& json) {
	::Model model;
	model.transform = MatrixIdentity();

	// Meshes
	model.meshCount = json["MESH"].size();
	model.meshes = new Mesh[model.meshCount];

	// Materials
	model.materials = new Material[ model.meshCount ];
	model.materialCount = model.meshCount;
	model.meshMaterial = new int[model.meshCount];

	// Convert the meshes
	for (int i=0; i < model.meshCount; i++) {
		Mesh mesh = {0};
		mesh.vertices = new float[ json["VERT"].get_binary().size() / 4 ];
		mesh.normals = new float[ json["NORM"].get_binary().size() / 4 ];
		mesh.texcoords = new float[ json["UV"].get_binary().size() / 4 ];

		mesh.vertexCount = json["VERT"].get_binary().size() / 4 / 3;
		mesh.triangleCount = mesh.vertexCount / 3;

		memcpy(mesh.vertices, json["VERT"].get_binary().data(), json["VERT"].get_binary().size());
		memcpy(mesh.normals, json["NORM"].get_binary().data(), json["NORM"].get_binary().size());
		memcpy(mesh.texcoords, json["UV"].get_binary().data(), json["UV"].get_binary().size());

		model.meshMaterial[i] = i;
		// model.materials[i] = level_materials[ j["MESH"][i]["MAT"] ];
		UploadMesh(&model.meshes[i], false);
	}
}

void HSE::add_level_collider(flecs::entity& entity) {
	// Create the body
	JPH::BodyCreationSettings settings(
		convert_mesh_shape( entity.get<HSE::Model>().data->model ),
		JPH::RVec3::sZero(),
		JPH::Quat::sIdentity(),
		JPH::EMotionType::Static,
		Layers::NON_MOVING
	);

	entity.set<HSE::Body>( HSE::Body(entity.world(), settings) );
}
