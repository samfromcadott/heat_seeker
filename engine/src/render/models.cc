#include <heat_seeker.hh>

void HSE::render_models(HSE::Model m, HSE::Position p) {
	DrawCube({p.x,p.y,p.z}, 2.0f, 2.0f, 2.0f, RED);
	DrawCubeWires({p.x,p.y,p.z}, 2.0f, 2.0f, 2.0f, MAROON);

	DrawGrid(10, 1.0f);
}
