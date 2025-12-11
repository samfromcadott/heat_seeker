#pragma once

#include <Jolt/Jolt.h>
#include <Jolt/RegisterTypes.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>
#include <Jolt/Physics/Collision/Shape/CylinderShape.h>
#include <Jolt/Physics/Collision/Shape/RotatedTranslatedShape.h>
#include <Jolt/Physics/Collision/PhysicsMaterialSimple.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Body/BodyActivationListener.h>
#include <Jolt/Physics/Collision/Shape/MeshShape.h>
#include <Jolt/Physics/Collision/RayCast.h>
#include <Jolt/Physics/Collision/CastResult.h>
#include <Jolt/Physics/Collision/Shape/CapsuleShape.h>
#include <Jolt/Physics/Character/CharacterBase.h>
#include <Jolt/Physics/Character/CharacterID.h>
#include <Jolt/Physics/Character/CharacterVirtual.h>

JPH_SUPPRESS_WARNINGS;

namespace HSE {

namespace Layers {
	static constexpr JPH::ObjectLayer NON_MOVING = 0;
	static constexpr JPH::ObjectLayer MOVING = 1;
	static constexpr JPH::ObjectLayer NUM_LAYERS = 2;
};

class ObjectLayerPairFilter : public JPH::ObjectLayerPairFilter {
public:
	virtual bool ShouldCollide(JPH::ObjectLayer inObject1, JPH::ObjectLayer inObject2) const override;
};

namespace BroadPhaseLayers {
	static constexpr JPH::BroadPhaseLayer NON_MOVING(0);
	static constexpr JPH::BroadPhaseLayer MOVING(1);
	static constexpr JPH::uint NUM_LAYERS(2);
};

class BroadPhaseLayerInterface final : public JPH::BroadPhaseLayerInterface {
public:
	BroadPhaseLayerInterface();

	virtual JPH::uint GetNumBroadPhaseLayers() const override;

	virtual JPH::BroadPhaseLayer GetBroadPhaseLayer(JPH::ObjectLayer inLayer) const override;

#if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
	virtual const char* GetBroadPhaseLayerName(JPH::BroadPhaseLayer inLayer) const override;
#endif // JPH_EXTERNAL_PROFILE || JPH_PROFILE_ENABLED

private:
	JPH::BroadPhaseLayer mObjectToBroadPhase[Layers::NUM_LAYERS];
};

class ObjectVsBroadPhaseLayerFilter : public JPH::ObjectVsBroadPhaseLayerFilter {
public:
	virtual bool ShouldCollide(JPH::ObjectLayer inLayer1, JPH::BroadPhaseLayer inLayer2) const override;
};

class ContactListener : public JPH::ContactListener {
public:
	// See: ContactListener
	virtual JPH::ValidateResult OnContactValidate(const JPH::Body &inBody1, const JPH::Body &inBody2, JPH::RVec3Arg inBaseOffset, const JPH::CollideShapeResult &inCollisionResult) override;

	virtual void OnContactAdded(const JPH::Body &inBody1, const JPH::Body &inBody2, const JPH::ContactManifold &inManifold, JPH::ContactSettings &ioSettings) override;

	virtual void OnContactPersisted(const JPH::Body &inBody1, const JPH::Body &inBody2, const JPH::ContactManifold &inManifold, JPH::ContactSettings &ioSettings) override;

	virtual void OnContactRemoved(const JPH::SubShapeIDPair &inSubShapePair) override;
};

class BodyActivationListener : public JPH::BodyActivationListener {
public:
	virtual void OnBodyActivated(const JPH::BodyID &inBodyID, JPH::uint64 inBodyUserData) override;

	virtual void OnBodyDeactivated(const JPH::BodyID &inBodyID, JPH::uint64 inBodyUserData) override;
};

class CharacterListener : public JPH::CharacterContactListener {
public:
	virtual void OnAdjustBodyVelocity(const JPH::CharacterVirtual *inCharacter, const JPH::Body &inBody2, JPH::Vec3 &ioLinearVelocity, JPH::Vec3 &ioAngularVelocity);
	virtual bool OnContactValidate(const JPH::CharacterVirtual *inCharacter, const JPH::BodyID &inBodyID2, const JPH::SubShapeID &inSubShapeID2);
	virtual bool OnCharacterContactValidate(const JPH::CharacterVirtual *inCharacter, const JPH::CharacterVirtual *inOtherCharacter, const JPH::SubShapeID &inSubShapeID2);
	virtual void OnContactAdded(const JPH::CharacterVirtual *inCharacter, const JPH::BodyID &inBodyID2, const JPH::SubShapeID &inSubShapeID2, JPH::RVec3Arg inContactPosition, JPH::Vec3Arg inContactNormal, JPH::CharacterContactSettings &ioSettings);
	virtual void OnContactPersisted(const JPH::CharacterVirtual *inCharacter, const JPH::BodyID &inBodyID2, const JPH::SubShapeID &inSubShapeID2, JPH::RVec3Arg inContactPosition, JPH::Vec3Arg inContactNormal, JPH::CharacterContactSettings &ioSettings);
	virtual void OnContactRemoved(const JPH::CharacterVirtual *inCharacter, const JPH::BodyID &inBodyID2, const JPH::SubShapeID &inSubShapeID2);
	virtual void OnCharacterContactAdded(const JPH::CharacterVirtual *inCharacter, const JPH::CharacterVirtual *inOtherCharacter, const JPH::SubShapeID &inSubShapeID2, JPH::RVec3Arg inContactPosition, JPH::Vec3Arg inContactNormal, JPH::CharacterContactSettings &ioSettings);
	virtual void OnCharacterContactPersisted(const JPH::CharacterVirtual *inCharacter, const JPH::CharacterVirtual *inOtherCharacter, const JPH::SubShapeID &inSubShapeID2, JPH::RVec3Arg inContactPosition, JPH::Vec3Arg inContactNormal, JPH::CharacterContactSettings &ioSettings);
	virtual void OnCharacterContactRemoved(const JPH::CharacterVirtual *inCharacter, const JPH::CharacterID &inOtherCharacterID, const JPH::SubShapeID &inSubShapeID2);
	virtual void OnContactSolve(const JPH::CharacterVirtual *inCharacter, const JPH::BodyID &inBodyID2, const JPH::SubShapeID &inSubShapeID2, JPH::RVec3Arg inContactPosition, JPH::Vec3Arg inContactNormal, JPH::Vec3Arg inContactVelocity, const JPH::PhysicsMaterial *inContactMaterial, JPH::Vec3Arg inCharacterVelocity, JPH::Vec3 &ioNewCharacterVelocity);
	virtual void OnCharacterContactSolve(const JPH::CharacterVirtual *inCharacter, const JPH::CharacterVirtual *inOtherCharacter, const JPH::SubShapeID &inSubShapeID2, JPH::RVec3Arg inContactPosition, JPH::Vec3Arg inContactNormal, JPH::Vec3Arg inContactVelocity, const JPH::PhysicsMaterial *inContactMaterial, JPH::Vec3Arg inCharacterVelocity, JPH::Vec3 &ioNewCharacterVelocity);
};

class PhysicsEngine {
private:
	flecs::world world;

	JPH::PhysicsSystem physics_system;
	JPH::TempAllocatorImpl* temp_allocator;
	JPH::JobSystemThreadPool job_system;
	BroadPhaseLayerInterface broad_phase_layer_interface;
	ObjectVsBroadPhaseLayerFilter object_vs_broadphase_layer_filter;
	ObjectLayerPairFilter object_vs_object_layer_filter;
	BodyActivationListener body_activation_listener;
	ContactListener contact_listener;

	const JPH::uint max_bodies = 65536;
	const JPH::uint body_mutexes = 0;
	const JPH::uint max_body_pairs = 1024;
	const JPH::uint max_contact_constraints = 1024;

public:
	PhysicsEngine();
	~PhysicsEngine();

	PhysicsEngine& operator=(const PhysicsEngine& other);

	void update();
	void set_world(flecs::world w);

	void set_gravity(const glm::vec3 g);
	glm::vec3 get_gravity() const;

	friend class Body;
	friend class CharacterBody;
};

class Body {
private:
	JPH::BodyID id;
	PhysicsEngine* engine = nullptr;

public:
	Body();
	Body(PhysicsEngine& engine, flecs::entity owner, const JPH::BodyCreationSettings& settings);
	~Body();

	void set_position(const glm::vec3& position);
	glm::vec3 get_position();

	void set_rotation(const glm::quat& rotation);
	glm::quat get_rotation();

	void set_velocity(const glm::vec3& velocity);
	glm::vec3 get_velocity();

	flecs::entity get_owner();
};

class CharacterBody {
private:
	JPH::CharacterVirtual* body = nullptr;
	PhysicsEngine* engine = nullptr;

public:
	CharacterBody();
	CharacterBody(PhysicsEngine& engine, flecs::entity owner, const JPH::CharacterVirtualSettings& settings);
	~CharacterBody();

	void set_position(const glm::vec3& position);
	glm::vec3 get_position();

	void set_rotation(const glm::quat& rotation);
	glm::quat get_rotation();

	void set_velocity(const glm::vec3& velocity);
	glm::vec3 get_velocity();

	flecs::entity get_owner();
};

// Type converters
inline JPH::Vec3 glm_to_jolt(glm::vec3 v) {
	return JPH::Vec3(v.x,v.y,v.z);
}

inline glm::vec3 jolt_to_glm(JPH::Vec3 v) {
	return glm::vec3( v.GetX(), v.GetY(), v.GetZ() );
}

inline JPH::Quat glm_to_jolt(glm::quat q) {
	return JPH::Quat(q.x,q.y,q.z,q.w);
}

inline glm::quat jolt_to_glm(JPH::Quat q) {
	return glm::quat( q.GetW(), q.GetX(), q.GetY(), q.GetZ() );
}

}
