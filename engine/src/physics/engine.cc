#include <heat_seeker.hh>

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
	// temp_allocator->Allocate(10 * 1024 * 1024);

	job_system.Init(JPH::cMaxPhysicsJobs, JPH::cMaxPhysicsBarriers, std::thread::hardware_concurrency() - 1);

	// Create the physics system
	physics_system.Init(max_bodies, body_mutexes, max_body_pairs, max_contact_constraints, broad_phase_layer_interface, object_vs_broadphase_layer_filter, object_vs_object_layer_filter);

	physics_system.SetBodyActivationListener(&body_activation_listener);
	physics_system.SetContactListener(&contact_listener);

	physics_system.SetGravity( JPH::Vec3Arg(0.0, 0.0, -9.8) );
}

PhysicsEngine::~PhysicsEngine() {
	JPH::UnregisterTypes();
	delete JPH::Factory::sInstance;
	JPH::Factory::sInstance = nullptr;

	delete temp_allocator;
}

PhysicsEngine& PhysicsEngine::operator=(const PhysicsEngine& other) {
	return *this;
}

void PhysicsEngine::update() {
	physics_system.Update(1.0/60.0, 1, temp_allocator, &job_system);
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
