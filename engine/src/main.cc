#include <heat_seeker.hh>

flecs::world HSE::Game;

using namespace HSE;

int main() {
	const int screenWidth = 1280;
	const int screenHeight = 720;

	InitWindow(screenWidth, screenHeight, "HEAT SEEKER");

	SetTargetFPS(60);

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
	e1.get_mut<Position>() = glm::vec3(1.0,0.0,3.0);

	flecs::entity e2 = Game.entity();
	e2.add<HSE::Model>();
	e2.add<Position>();
	e2.add<Rotation>();
	e2.get_mut<Position>() = glm::vec3(-1.0,2.0,-5.0);
	e2.get_mut<Rotation>() = glm::quat(0.9238795, 0, 0.3826834, 0);

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
