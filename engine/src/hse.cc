#include <heat_seeker.hh>

void HSE::init(const std::string& title, const int width, const int height) {
	InitWindow( width, height, title.c_str() );
	SetTargetFPS(60);
}

void HSE::quit() {
	// Unload models
	for (auto [key, model] : model_files) {
		model.unload();
	}

	UnloadShader(gouraud_shader);
	CloseWindow();
}
