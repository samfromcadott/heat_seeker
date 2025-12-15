#include <heat_seeker.hh>

using namespace HSE;

Body::Body() {

}

Body::Body(flecs::world world, const JPH::BodyCreationSettings& settings) {
	this->engine = &world.get_mut<PhysicsEngine>();
	id = this->engine->physics_system.GetBodyInterface().CreateAndAddBody(settings, JPH::EActivation::Activate);
}

Body::~Body() {

}

void Body::set_position(const glm::vec3& position) {
	engine->physics_system.GetBodyInterface().SetPosition(id, glm_to_jolt(position), JPH::EActivation::Activate);
}

glm::vec3 Body::get_position() {
	return jolt_to_glm( engine->physics_system.GetBodyInterface().GetPosition(id) );
}

void Body::set_rotation(const glm::quat& rotation) {
	engine->physics_system.GetBodyInterface().SetRotation(id, glm_to_jolt(rotation), JPH::EActivation::Activate);
}

glm::quat Body::get_rotation() {
	return jolt_to_glm( engine->physics_system.GetBodyInterface().GetRotation(id) );
}

void Body::set_velocity(const glm::vec3& velocity) {
	engine->physics_system.GetBodyInterface().SetLinearVelocity( id, glm_to_jolt(velocity) );
}

glm::vec3 Body::get_velocity() {
	return jolt_to_glm( engine->physics_system.GetBodyInterface().GetLinearVelocity(id) );
}

void Body::set_owner(flecs::entity owner) {
	if ( id.IsInvalid() ) return;
	engine->physics_system.GetBodyInterface().SetUserData( id, owner.id() );
}

flecs::entity Body::get_owner() {
	auto owner = engine->physics_system.GetBodyInterface().GetUserData(id);
	return flecs::entity(engine->world, owner);
}
