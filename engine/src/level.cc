#include <heat_seeker.hh>

using namespace HSE;
using namespace std;
using namespace nlohmann;
using namespace flecs::meta;

void HSE::load_level(flecs::world& world, const string& filename) {
	std::cout << "Loading map " << filename << '\n';
	std::ifstream map_file(filename); // Load map
	auto json_file = json::from_cbor(map_file); // Convert to JSON object

	// Loop through entities in ENT
	for ( const auto& [name, e] : json_file["ENT"].items() ) {
		std::cout << "Parsing entity " << name << '\n';
		// std::string n = name;
		flecs::entity entity = parse_entity(world, name, e);
		// parse_entity(world, name, e);
		// entity.set_name( name.c_str() );
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

	if (type == PrimitiveType) {
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

	else if (type == StructType) {
		cur.push();
		for ( const auto& [key, value] : json.items() ) {
			cur.member( key.c_str() );
			parse_component_member(world, value, cur);
		}
		cur.pop();
	}

	else if (type == EnumType) {
		auto& enum_vec = cur.get_type().get<EcsConstants>().ordered_constants;
		for (int i = 0; i < enum_vec.count; i++) {
			auto& member = reinterpret_cast<ecs_enum_constant_t*>(enum_vec.array)[i];
			if ( json != string(member.name) ) continue;

			cur.set_uint(member.value_unsigned);
			break;
		}
	}

	else if (type == CustomType) {
		if ( json.is_string() )
			cur.set_string( string(json).c_str() );
	}
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
