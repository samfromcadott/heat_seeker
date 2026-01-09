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
	}

	if ( options.translation != glm::vec3(0.0, 0.0, 0.0) or options.rotation != glm::quat(1.0, 0.0, 0.0, 0.0) ) {
		return new JPH::RotatedTranslatedShape(glm_to_jolt(options.translation), glm_to_jolt(options.rotation), shape);
	}

	return shape;
}
