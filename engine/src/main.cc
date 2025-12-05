#include <heat_seeker.hh>

flecs::world HSE::Game;

using namespace HSE;

int main() {
	const int screenWidth = 1280;
	const int screenHeight = 720;

	InitWindow(screenWidth, screenHeight, "HEAT SEEKER");

	SetTargetFPS(60);

	Game.system().each(start_render);
	Game.system().each(start_3D);
	Game.system<HSE::Model, HSE::Position>().each(render_models);
	Game.system().each(end_3D);
	Game.system().each(end_render);

	flecs::entity e1 = Game.entity();
	e1.add<HSE::Model>();
	e1.add<Position>();
	auto& p = e1.get_mut<Position>();
	p = glm::vec3(1.0,0.0,3.0);

	// Set e1's model
	::Model sphere = LoadModelFromMesh( GenMeshSphere(1.0, 4, 8) );
	ModelData md;
	md.model = sphere;
	e1.get_mut<HSE::Model>().data = &md;

	// Main game loop
	while ( !WindowShouldClose() ) {
		Game.progress();
	}

	CloseWindow();

	return 0;
}
