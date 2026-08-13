#include <heat_seeker.hh>

using namespace HSE;
using namespace std;
using namespace nlohmann;

void HSE::load_level(flecs::world& world, const string& filename) {
	std::cout << "Loading map " << filename << '\n';
	auto map_file = File::open(filename);
	auto json_file = json::from_cbor(map_file); // Convert to JSON object

	// Loop through entities in ENT
	for ( const auto& [name, e] : json_file["ENT"].items() ) {
		std::cout << "Parsing entity " << name << '\n';
		flecs::entity entity = parse_entity(world, name, e);
	}
}

flecs::entity HSE::parse_entity(flecs::world& world, const std::string& name, const nlohmann::json& json) {
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

	entity.set_name( name.c_str() );

	// If it has a model convert and add it
	if ( json.contains("MODEL") ) {
		add_level_model(entity, json["MODEL"]);
		add_level_collider(entity);
	}

	// Loop through keys
	for ( const auto& [key, value] : json["COMP"].items() ) {
		parse_component(entity, key, value);
	}

	return entity;
}

void HSE::add_level_model(flecs::entity& entity, const nlohmann::json& json) {
	Asset<ModelData> data( entity.name().c_str() );
	data.make_new();

	auto& model = data->model;
	model = {0};
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
		auto& m = json["MESH"][i];
		Mesh mesh = {0};

		mesh.vertices = new float[ m["VERT"].get_binary().size() / sizeof(float) ];
		mesh.normals = new float[ m["NORM"].get_binary().size() / sizeof(float) ];
		mesh.texcoords = new float[ m["UV"].get_binary().size() / sizeof(float) ];

		mesh.vertexCount = m["VERT"].get_binary().size() / sizeof(float) / 3;
		mesh.triangleCount = mesh.vertexCount / 3;

		memcpy(mesh.vertices, m["VERT"].get_binary().data(), m["VERT"].get_binary().size());
		memcpy(mesh.normals, m["NORM"].get_binary().data(), m["NORM"].get_binary().size());
		memcpy(mesh.texcoords, m["UV"].get_binary().data(), m["UV"].get_binary().size());

		model.materials[i] = load_level_material( m["MAT"] );
		model.meshMaterial[i] = i;
		model.meshes[i] = mesh;
		UploadMesh(&model.meshes[i], false);
	}

	entity.add<HSE::Model>();
	entity.get_mut<HSE::Model>().data = data;
	std::cout << "Model \"" << entity.name().c_str() << "\" created\n";
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

Material HSE::load_level_material(const std::string& name) {
	Material mat = LoadMaterialDefault();
	mat.shader = gouraud_shader;
	mat.maps[MATERIAL_MAP_DIFFUSE].texture = LoadTexture( name.c_str() );

	return mat;
}
