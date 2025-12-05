#include <heat_seeker.hh>

void HSE::render_models(HSE::Model m, HSE::Position p) {
	// DrawCube(p, 2.0f, 2.0f, 2.0f, RED);
	// DrawCubeWires(p, 2.0f, 2.0f, 2.0f, MAROON);
	DrawModelWires(m.data->model, p, 1.0, BLACK);

	DrawGrid(10, 1.0f);
}
