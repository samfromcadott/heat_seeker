#include <heat_seeker.hh>

Color HSE::clear_color = GRAY;
std::function<void(void)> HSE::ui_function;
std::map<std::string, Shader> HSE::shaders;
std::string HSE::default_shader;

void HSE::start_render() {
	BeginDrawing();
	ClearBackground(clear_color);
}

void HSE::start_3D() {
	BeginMode3D(HSE::camera);
}

void HSE::end_render() {
	EndDrawing();
}

void HSE::end_3D() {
	EndMode3D();
}

void HSE::update_ui() {
	if(ui_function) ui_function();
}
