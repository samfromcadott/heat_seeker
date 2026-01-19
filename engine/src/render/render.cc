#include <heat_seeker.hh>

std::function<void(void)> HSE::ui_function;

// Camera3D camera = {
// 	.position = Vector3 { 0.0f, 10.0f, 10.0f },
// 	.target = Vector3 { 0.0f, 0.0f, 0.0f },
// 	.up = Vector3 { 0.0f, 1.0f, 0.0f },
// 	.fovy = 45.0f,
// 	.projection = CAMERA_PERSPECTIVE,
// };

void HSE::start_render() {
	BeginDrawing();
	ClearBackground(RAYWHITE);
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
