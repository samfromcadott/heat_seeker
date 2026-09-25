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
		if ( not level_entity_has_tag(json, "NO_RENDER") )
			add_level_model(entity, json["MODEL"]);

		if ( not level_entity_has_tag(json, "NO_COLLIDE") ) {
			if ( level_entity_has_tag(json, "CONVEX") )
				add_level_collider_convex(entity, json["MODEL"]);
			else
				add_level_collider(entity, json["MODEL"]);
		}
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
	data->model = read_level_model(json);

	entity.add<HSE::Model>();
	entity.get_mut<HSE::Model>().data = data;
	std::cout << "Model \"" << entity.name().c_str() << "\" created\n";
}

void HSE::add_level_collider(flecs::entity& entity, const nlohmann::json& json) {
	// Create the body
	auto model = read_level_model(json);
	JPH::BodyCreationSettings settings(
		convert_mesh_shape(model),
		JPH::RVec3::sZero(),
		JPH::Quat::sIdentity(),
		JPH::EMotionType::Static,
		JPH::ObjectLayerPairFilterMask::sGetObjectLayer(
			Layers::NON_MOVING, Layers::MOVING
		)
	);
	UnloadModel(model);

	entity.set<HSE::Body>( HSE::Body(entity.world(), settings) );
}

void HSE::add_level_collider_convex(flecs::entity& entity, const nlohmann::json& json) {
	JPH::Array<JPH::Vec3> points;

	// Convert meshes
	for (const auto& mesh : json["MESH"]) {
		// Loop over vertices
		const auto& bin = mesh["VERT"].get_binary();
		for (int i = 0; i < bin.size(); i+=12) {
			// Convert bytes to floats
			float x,y,z;
			memcpy( &x, &bin[i+0], sizeof(float) );
			memcpy( &y, &bin[i+4], sizeof(float) );
			memcpy( &z, &bin[i+8], sizeof(float) );

			points.push_back( JPH::Vec3(x,y,z) );
		}
	}

	auto* shape = new JPH::ConvexHullShapeSettings(points);
	JPH::BodyCreationSettings settings(
			shape,
			JPH::RVec3::sZero(),
			JPH::Quat::sIdentity(),
			JPH::EMotionType::Static,
			JPH::ObjectLayerPairFilterMask::sGetObjectLayer(
			Layers::NON_MOVING, Layers::MOVING
		)
	);
	// delete shape;
	settings.mAllowDynamicOrKinematic = true; // This is to allow model to be made moving (like doors)

	entity.set<HSE::Body>( HSE::Body(entity.world(), settings) );
}

::Model HSE::read_level_model(const nlohmann::json& json) {
	::Model model = {0};
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

	return model;
}

bool HSE::level_entity_has_tag(const nlohmann::json& json, const std::string& tag) {
	if ( not json.contains("TAGS") ) return false;
	if ( std::find(json["TAGS"].begin(), json["TAGS"].end(), tag) == json["TAGS"].end() )
		return false;

	return true;
}

Material HSE::load_level_material(const std::string& name) {
	Material mat = LoadMaterialDefault();
	mat.shader = gouraud_shader;
	mat.maps[MATERIAL_MAP_DIFFUSE].texture = LoadTexture( name.c_str() );

	return mat;
}
