#include <heat_seeker.hh>

flecs::world HSE::Game;

using namespace HSE;

int main() {
	const int screenWidth = 1280;
	const int screenHeight = 720;

	InitWindow(screenWidth, screenHeight, "HEAT SEEKER");

	SetTargetFPS(60);

	// Add physics engine
	Game.component<PhysicsEngine>().add(flecs::Singleton);
	Game.set<PhysicsEngine>( PhysicsEngine() );
	Game.get_mut<PhysicsEngine>().set_world(Game);
	Game.get_mut<PhysicsEngine>().set_gravity( glm::vec3(0.0, -9.8, 0.0) );

	// Register systems
	Game.system<Body&, Position&>().each(pos_to_body);
	Game.system<Body&, Rotation&>().each(rot_to_body);
	Game.system<PhysicsEngine&>().each(physics_update);
	Game.system<Body&, Position&>().each(body_to_pos);
	Game.system<Body&, Rotation&>().each(body_to_rot);

	Game.system().each(start_render);
	Game.system().each(start_3D);
	Game.system<HSE::Model&, HSE::Position&, HSE::Rotation&>().each(render_models);
	Game.system().each(end_3D);
	Game.system().each(end_render);

	// Create a couple entities
	flecs::entity e1 = Game.entity();
	e1.add<HSE::Model>();
	e1.add<Position>();
	e1.add<Rotation>();
	e1.get_mut<Position>() = glm::vec3(0.5,5.0,0.0);
	e1.add<Body>();
	e1.set<Body>( Body(Game.get_mut<PhysicsEngine>(), e1, JPH::BodyCreationSettings(
		new JPH::SphereShape(1.0),
		JPH::RVec3::sZero(),
		JPH::Quat::sIdentity(),
		JPH::EMotionType::Dynamic,
		Layers::MOVING
	)));

	flecs::entity e2 = Game.entity();
	e2.add<HSE::Model>();
	e2.add<Position>();
	e2.add<Rotation>();
	e2.get_mut<Position>() = glm::vec3(0.0,0.0,0.0);
	e2.get_mut<Rotation>() = glm::quat(0.9238795, 0, 0.3826834, 0);
	e2.add<Body>();
	e2.set<Body>( Body(Game.get_mut<PhysicsEngine>(), e1, JPH::BodyCreationSettings(
		new JPH::SphereShape(1.0),
		JPH::RVec3::sZero(),
		JPH::Quat::sIdentity(),
		JPH::EMotionType::Static,
		Layers::NON_MOVING
	)));

	// Set their models
	::Model sphere = LoadModelFromMesh( GenMeshSphere(1.0, 4, 8) );
	ModelData md;
	md.model = sphere;
	e1.get_mut<HSE::Model>().data = &md;
	e2.get_mut<HSE::Model>().data = &md;

	// Main game loop
	while ( !WindowShouldClose() ) {
		Game.progress();
	}

	CloseWindow();

	return 0;
}
