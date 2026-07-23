#include <heat_seeker.hh>

JPH::Ref<JPH::Shape> HSE::convert_shape(const ShapeOptions& options) {
	JPH::Ref<JPH::Shape> shape;

	switch (options.type) {
		case ShapeType::NONE:
			shape = new JPH::EmptyShape();
			break;
		case ShapeType::SPHERE:
			shape = new JPH::SphereShape(options.radius);
			break;
		case ShapeType::BOX:
			shape = new JPH::BoxShape( glm_to_jolt(options.size / 2.0f) );
			break;
		case ShapeType::CYLINDER:
			shape = new JPH::CylinderShape(options.height / 2.0, options.radius);
			break;
		case ShapeType::CAPSULE:
			shape = new JPH::CapsuleShape(options.height / 2.0, options.radius);
			break;
		case ShapeType::MESH:
			auto model = Asset<ModelData>(options.file);
			model.load();
			shape = convert_mesh_shape( model->model );
			break;
	}

	if ( options.translation != glm::vec3(0.0, 0.0, 0.0) or options.rotation != glm::quat(1.0, 0.0, 0.0, 0.0) ) {
		return new JPH::RotatedTranslatedShape(glm_to_jolt(options.translation), glm_to_jolt(options.rotation), shape);
	}

	return shape;
}

JPH::Ref<JPH::Shape> HSE::convert_mesh_shape(const ::Model& model) {
	// Convert each mesh to a Jolt mesh shape
	JPH::VertexList verts;
	JPH::IndexedTriangleList tris;
	int index_offset = 0; // Offest of vertex indecies when starting a new mesh

	for (int i = 0; i < model.meshCount; i++) {
		// Add vertices to the mesh
		for (int j = 0; j < model.meshes[i].vertexCount*3; j+=3) {
			const auto& v1 = model.meshes[i].vertices[j+0];
			const auto& v2 = model.meshes[i].vertices[j+1];
			const auto& v3 = model.meshes[i].vertices[j+2];
			verts.push_back( JPH::Float3(v1,v2,v3) );
		}

		for (int k = 0; k < model.meshes[i].triangleCount; k++) {
			tris.push_back( JPH::IndexedTriangle(k*3+0+index_offset, k*3+1+index_offset, k*3+2+index_offset) );
		}

		index_offset += model.meshes[i].vertexCount;
	}

	JPH::MeshShapeSettings settings;
	if (tris.size() > 0) settings = JPH::MeshShapeSettings(verts, tris);
	settings.Sanitize();
	JPH::Shape::ShapeResult result;

	return new JPH::MeshShape(settings, result);
}
