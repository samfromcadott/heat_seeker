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
	Game.system().each(render_models);
	Game.system().each(end_3D);
	Game.system().each(end_render);

	// Main game loop
	while ( !WindowShouldClose() ) {
		Game.progress();
	}

	CloseWindow();

	return 0;
}
