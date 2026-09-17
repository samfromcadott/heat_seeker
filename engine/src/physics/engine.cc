#include <heat_seeker.hh>
#include <Jolt/Physics/Collision/ShapeCast.h>

using namespace HSE;

// Callback for traces, connect this to your own trace function if you have one
static void TraceImpl(const char *inFMT, ...) {
	// Format the message
	va_list list;
	va_start(list, inFMT);
	char buffer[1024];
	vsnprintf(buffer, sizeof(buffer), inFMT, list);
	va_end(list);

	// Print to the TTY
	std::cout << buffer << '\n';
}

#ifdef JPH_ENABLE_ASSERTS

// Callback for asserts, connect this to your own assert handler if you have one
static bool AssertFailedImpl(const char *inExpression, const char *inMessage, const char *inFile, JPH::uint inLine) {
	// Print to the TTY
	std::cout << inFile << ":" << inLine << ": (" << inExpression << ") " << (inMessage != nullptr? inMessage : "") << '\n';

	// Breakpoint
	return true;
};

#endif // JPH_ENABLE_ASSERTS

PhysicsEngine::PhysicsEngine() {
	// Physics system setup
	JPH::RegisterDefaultAllocator();

	JPH::Trace = TraceImpl;
	JPH_IF_ENABLE_ASSERTS(JPH::AssertFailed = AssertFailedImpl;)

	JPH::Factory::sInstance = new JPH::Factory();

	JPH::RegisterTypes();

	temp_allocator = new JPH::TempAllocatorImpl(10 * 1024 * 1024);
	bp_interface = new JPH::BroadPhaseLayerInterfaceMask(8);
	bp_filter = new JPH::ObjectVsBroadPhaseLayerFilterMask(*bp_interface);
	pair_filter = new JPH::ObjectLayerPairFilterMask;

	job_system.Init(JPH::cMaxPhysicsJobs, JPH::cMaxPhysicsBarriers, std::thread::hardware_concurrency() - 1);

	// Create the physics system
	physics_system.Init(max_bodies, body_mutexes, max_body_pairs, max_contact_constraints, *bp_interface, *bp_filter, *pair_filter);

	physics_system.SetBodyActivationListener(&body_activation_listener);
	physics_system.SetContactListener(&contact_listener);

	physics_system.SetGravity( JPH::Vec3Arg(0.0, 0.0, -9.8) );
	bp_interface->ConfigureLayer(BP_UNIFIED, Layers::MOVING | Layers::NON_MOVING, 0);
}

PhysicsEngine::~PhysicsEngine() {
	JPH::UnregisterTypes();
	delete JPH::Factory::sInstance;
	JPH::Factory::sInstance = nullptr;

	delete temp_allocator;
	delete bp_interface;
	delete bp_filter;
	delete pair_filter;
}

PhysicsEngine& PhysicsEngine::operator=(const PhysicsEngine& other) {
	return *this;
}

void PhysicsEngine::update() {
	physics_system.Update(1.0/60.0, 4, temp_allocator, &job_system);
}

void PhysicsEngine::set_world(flecs::world w) {
	world = w;
}

void PhysicsEngine::set_gravity(const vec3 g) {
	physics_system.SetGravity( glm_to_jolt(g) );
}

vec3 PhysicsEngine::get_gravity() const {
	return jolt_to_glm( physics_system.GetGravity() );
}

JPH::PhysicsSystem& PhysicsEngine::get_system() {
	return physics_system;
}

RayCastHit PhysicsEngine::ray_cast(vec3 origin, vec3 ray) const {
	JPH::RayCast r( glm_to_jolt(origin), glm_to_jolt(ray) );

	JPH::AllHitCollisionCollector<JPH::RayCastBodyCollector> collector;
	physics_system.GetBroadPhaseQuery().CastRay(r, collector);

	RayCastHit hit;
	hit.hit = collector.mHits.size() > 0;

	for (auto& h : collector.mHits) {
		hit.positions.push_back( origin + ray * h.mFraction );
		auto entity_id = physics_system.GetBodyInterface().GetUserData(h.mBodyID);
		hit.entities.push_back( flecs::entity(world, entity_id) );
	}

	return hit;
}

std::vector<Entity> PhysicsEngine::shape_cast(const vec3 origin, ShapeOptions shape_options) const {
	std::vector<Entity> entities;

	JPH::ShapeCastSettings settings;
	JPH::Ref<JPH::Shape> shape = convert_shape(shape_options);
	JPH::RShapeCast s {
		shape,
		JPH::Vec3::sOne(),
		JPH::RMat44::sTranslation( glm_to_jolt(origin) ),
		JPH::Vec3(0, 0, 1)
	};

	JPH::AllHitCollisionCollector<JPH::CastShapeCollector> collector;
	physics_system.GetNarrowPhaseQuery().CastShape(s, settings, JPH::RVec3::sZero(), collector);

	for (auto& h : collector.mHits) {
		auto entity_id = physics_system.GetBodyInterface().GetUserData(h.mBodyID2);
		entities.push_back( flecs::entity(world, entity_id) );
	}

	return entities;
}
