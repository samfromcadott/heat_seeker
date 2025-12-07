#include <heat_seeker.hh>

using namespace HSE;

JPH::ValidateResult ContactListener::OnContactValidate(const JPH::Body &inBody1, const JPH::Body &inBody2, JPH::RVec3Arg inBaseOffset, const JPH::CollideShapeResult &inCollisionResult) {
	return JPH::ValidateResult::AcceptAllContactsForThisBodyPair;
}

void ContactListener::OnContactAdded(const JPH::Body &inBody1, const JPH::Body &inBody2, const JPH::ContactManifold &inManifold, JPH::ContactSettings &ioSettings) {

}

void ContactListener::OnContactPersisted(const JPH::Body &inBody1, const JPH::Body &inBody2, const JPH::ContactManifold &inManifold, JPH::ContactSettings &ioSettings) {

}

void ContactListener::OnContactRemoved(const JPH::SubShapeIDPair &inSubShapePair) {

}

void BodyActivationListener::OnBodyActivated(const JPH::BodyID &inBodyID, JPH::uint64 inBodyUserData) {

}

void BodyActivationListener::OnBodyDeactivated(const JPH::BodyID &inBodyID, JPH::uint64 inBodyUserData) {

}
