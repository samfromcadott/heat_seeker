#include <heat_seeker.hh>

using namespace HSE;

CharacterBody::CharacterBody() {

}

CharacterBody::CharacterBody(flecs::world world, const JPH::CharacterVirtualSettings& settings) {
	this->engine = &world.get_mut<PhysicsEngine>();
	body = new JPH::CharacterVirtual(
		&settings,
		JPH::RVec3::sZero(),
		JPH::Quat::sIdentity(),
		&(engine->physics_system)
	);
}

CharacterBody::CharacterBody(flecs::world world, const CharacterBodyOptions& options) {
	this->engine = &world.get_mut<PhysicsEngine>();

	JPH::CharacterVirtualSettings settings;
	settings.mMass = options.mass;
	settings.mMaxStrength = options.max_strength;
	settings.mMaxSlopeAngle = options.max_slope;
	settings.mUp = glm_to_jolt(options.up);
	settings.mShape = convert_shape(options.shape);

	body = new JPH::CharacterVirtual(
		&settings,
		JPH::RVec3::sZero(),
		JPH::Quat::sIdentity(),
		&(engine->physics_system)
	);
}

CharacterBody::~CharacterBody() {

}

void CharacterBody::update() {
	// Gravity update
	if ( !on_floor() ) {
		auto v = body->GetLinearVelocity();
		v += body->GetUp() * engine->physics_system.GetGravity() * 1.0/60.0;
		body->SetLinearVelocity(v);
	}

	JPH::CharacterVirtual::ExtendedUpdateSettings update_settings;
	body->ExtendedUpdate(
		1.0/60.0,
		-body->GetUp() * engine->physics_system.GetGravity().Length(),
		update_settings,
		engine->physics_system.GetDefaultBroadPhaseLayerFilter(Layers::MOVING),
		engine->physics_system.GetDefaultLayerFilter(Layers::MOVING),
		{ },
		{ },
		*(engine->temp_allocator)
	);
}

bool CharacterBody::on_floor() {
	return body->IsSupported();
}

void CharacterBody::set_position(const glm::vec3& position) {
	body->SetPosition( glm_to_jolt(position) );
}

glm::vec3 CharacterBody::get_position() {
	return jolt_to_glm( body->GetPosition() );
}


void CharacterBody::set_rotation(const glm::quat& rotation) {
	body->SetRotation( glm_to_jolt(rotation) );
}

glm::quat CharacterBody::get_rotation() {
	return jolt_to_glm( body->GetRotation() );
}

void CharacterBody::set_velocity(const glm::vec3& velocity) {
	body->SetLinearVelocity( glm_to_jolt(velocity) );
}

glm::vec3 CharacterBody::get_velocity() {
	return jolt_to_glm( body->GetLinearVelocity() );
}

void CharacterBody::set_owner(flecs::entity owner) {
	if (!body) return;
	body->SetUserData( owner.id() );
}

flecs::entity CharacterBody::get_owner() {
	auto owner = body->GetUserData();
	return flecs::entity(engine->world, owner);
}
