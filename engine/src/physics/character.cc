#include <heat_seeker.hh>

using namespace HSE;

CharacterBody::CharacterBody() {

}

CharacterBody::CharacterBody(PhysicsEngine& engine, flecs::entity owner, const JPH::CharacterVirtualSettings& settings) {
	body = new JPH::CharacterVirtual(
		&settings,
		JPH::RVec3::sZero(),
		JPH::Quat::sIdentity(),
		owner.id(),
		&(engine.physics_system)
	);
	this->engine = &engine;
}

CharacterBody::~CharacterBody() {

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

flecs::entity CharacterBody::get_owner() {
	auto owner = body->GetUserData();
	return flecs::entity(engine->world, owner);
}
