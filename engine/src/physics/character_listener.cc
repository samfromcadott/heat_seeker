#include <heat_seeker.hh>

using namespace HSE;

void CharacterListener::OnAdjustBodyVelocity(const JPH::CharacterVirtual* inCharacter, const JPH::Body& inBody2, JPH::Vec3& ioLinearVelocity, JPH::Vec3& ioAngularVelocity) {

}

bool CharacterListener::OnContactValidate(const JPH::CharacterVirtual* inCharacter, const JPH::BodyID& inBodyID2, const JPH::SubShapeID& inSubShapeID2) {
	return true;
}

bool CharacterListener::OnCharacterContactValidate(const JPH::CharacterVirtual* inCharacter, const JPH::CharacterVirtual* inOtherCharacter, const JPH::SubShapeID& inSubShapeID2) {
	return true;
}

void CharacterListener::OnContactAdded(const JPH::CharacterVirtual* inCharacter, const JPH::BodyID& inBodyID2, const JPH::SubShapeID& inSubShapeID2, JPH::RVec3Arg inContactPosition, JPH::Vec3Arg inContactNormal, JPH::CharacterContactSettings& ioSettings) {
	auto ent_1 = flecs::entity( Game, inCharacter->GetUserData() );
	auto ent_2 = flecs::entity( Game, Game.get_mut<PhysicsEngine>().get_system().GetBodyInterface().GetUserData(inBodyID2) );

	ent_1.set<ContactAdded>({ ent_2 });
	ent_2.set<ContactAdded>({ ent_1 });
}

void CharacterListener::OnContactPersisted(const JPH::CharacterVirtual* inCharacter, const JPH::BodyID& inBodyID2, const JPH::SubShapeID& inSubShapeID2, JPH::RVec3Arg inContactPosition, JPH::Vec3Arg inContactNormal, JPH::CharacterContactSettings& ioSettings) {

}

void CharacterListener::OnContactRemoved(const JPH::CharacterVirtual* inCharacter, const JPH::BodyID& inBodyID2, const JPH::SubShapeID& inSubShapeID2) {

}

void CharacterListener::OnCharacterContactAdded(const JPH::CharacterVirtual* inCharacter, const JPH::CharacterVirtual* inOtherCharacter, const JPH::SubShapeID& inSubShapeID2, JPH::RVec3Arg inContactPosition, JPH::Vec3Arg inContactNormal, JPH::CharacterContactSettings& ioSettings) {
	auto ent_1 = flecs::entity( Game, inCharacter->GetUserData() );
	auto ent_2 = flecs::entity( Game, inOtherCharacter->GetUserData() );

	ent_1.set<ContactAdded>({ ent_2 });
	ent_2.set<ContactAdded>({ ent_1 });
}

void CharacterListener::OnCharacterContactPersisted(const JPH::CharacterVirtual* inCharacter, const JPH::CharacterVirtual* inOtherCharacter, const JPH::SubShapeID& inSubShapeID2, JPH::RVec3Arg inContactPosition, JPH::Vec3Arg inContactNormal, JPH::CharacterContactSettings& ioSettings) {

}

void CharacterListener::OnCharacterContactRemoved(const JPH::CharacterVirtual* inCharacter, const JPH::CharacterID& inOtherCharacterID, const JPH::SubShapeID& inSubShapeID2) {

}

void CharacterListener::OnContactSolve(const JPH::CharacterVirtual* inCharacter, const JPH::BodyID& inBodyID2, const JPH::SubShapeID& inSubShapeID2, JPH::RVec3Arg inContactPosition, JPH::Vec3Arg inContactNormal, JPH::Vec3Arg inContactVelocity, const JPH::PhysicsMaterial *inContactMaterial, JPH::Vec3Arg inCharacterVelocity, JPH::Vec3& ioNewCharacterVelocity) {

}

void CharacterListener::OnCharacterContactSolve(const JPH::CharacterVirtual* inCharacter, const JPH::CharacterVirtual* inOtherCharacter, const JPH::SubShapeID& inSubShapeID2, JPH::RVec3Arg inContactPosition, JPH::Vec3Arg inContactNormal, JPH::Vec3Arg inContactVelocity, const JPH::PhysicsMaterial *inContactMaterial, JPH::Vec3Arg inCharacterVelocity, JPH::Vec3& ioNewCharacterVelocity) {

}
