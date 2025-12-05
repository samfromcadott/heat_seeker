#include <heat_seeker.hh>

void HSE::render_models() {
	DrawCube({0.0,0.0,0.0}, 2.0f, 2.0f, 2.0f, RED);
	DrawCubeWires({0.0,0.0,0.0}, 2.0f, 2.0f, 2.0f, MAROON);

	DrawGrid(10, 1.0f);
}
