#include <heat_seeker.hh>

void HSE::init(const std::string& title, const int width, const int height) {
	InitWindow( width, height, title.c_str() );
	SetTargetFPS(60);
}

void HSE::quit() {
	UnloadShader(gouraud_shader);
	CloseWindow();
}
