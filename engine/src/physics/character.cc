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
	body->SetListener(&engine->character_listener);
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
	body->SetListener(&engine->character_listener);

	gravity_scale = options.gravity_scale;
}

CharacterBody::~CharacterBody() {

}

void CharacterBody::update() {
	// Gravity update
	if ( !on_floor() ) {
		auto v = body->GetLinearVelocity();
		v += engine->physics_system.GetGravity()  * gravity_scale * 1.0/60.0;
		body->SetLinearVelocity(v);
	}

	JPH::CharacterVirtual::ExtendedUpdateSettings update_settings;
	body->ExtendedUpdate(
		1.0/60.0,
		engine->physics_system.GetGravity() * gravity_scale,
		update_settings,
		engine->physics_system.GetDefaultBroadPhaseLayerFilter(Layers::MOVING),
		engine->physics_system.GetDefaultLayerFilter(Layers::MOVING),
		{ },
		{ },
		*(engine->temp_allocator)
	);
}

bool CharacterBody::on_floor() const {
	return body->IsSupported();
}

void CharacterBody::set_position(const vec3& position) {
	body->SetPosition( glm_to_jolt(position) );
}

vec3 CharacterBody::get_position() const {
	return jolt_to_glm( body->GetPosition() );
}


void CharacterBody::set_rotation(const quat& rotation) {
	body->SetRotation( glm_to_jolt(rotation) );
}

quat CharacterBody::get_rotation() const {
	return jolt_to_glm( body->GetRotation() );
}

void CharacterBody::set_velocity(const vec3& velocity) {
	body->SetLinearVelocity( glm_to_jolt(velocity) );
}

vec3 CharacterBody::get_velocity() const {
	return jolt_to_glm( body->GetLinearVelocity() );
}

void CharacterBody::set_owner(flecs::entity owner) {
	if (!body) return;
	body->SetUserData( owner.id() );
}

flecs::entity CharacterBody::get_owner() const {
	auto owner = body->GetUserData();
	return flecs::entity(engine->world, owner);
}
