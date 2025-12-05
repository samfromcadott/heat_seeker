#include <heat_seeker.hh>

int main() {
	flecs::world Game;

	Game.system<>().each([]() {
		std::cout << "New frame" << '\n';
	});

	const int screenWidth = 1280;
	const int screenHeight = 720;

	InitWindow(screenWidth, screenHeight, "HEAT SEEKER");

	// Define the camera to look into our 3d world
	Camera3D camera = { 0 };
	camera.position = Vector3 { 0.0f, 10.0f, 10.0f }; // Camera position
	camera.target = Vector3 { 0.0f, 0.0f, 0.0f }; // Camera looking at point
	camera.up = Vector3 { 0.0f, 1.0f, 0.0f }; // Camera up vector (rotation towards target)
	camera.fovy = 45.0f; // Camera field-of-view Y
	camera.projection = CAMERA_PERSPECTIVE; // Camera mode type

	Vector3 cubePosition = { 0.0f, 0.0f, 0.0f };

	SetTargetFPS(60);

	// Main game loop
	while ( !WindowShouldClose() ) {
		Game.progress();

		BeginDrawing();

			ClearBackground(RAYWHITE);

			BeginMode3D(camera);

				DrawCube(cubePosition, 2.0f, 2.0f, 2.0f, RED);
				DrawCubeWires(cubePosition, 2.0f, 2.0f, 2.0f, MAROON);

				DrawGrid(10, 1.0f);

			EndMode3D();

			DrawText("Welcome to the third dimension!", 10, 40, 20, DARKGRAY);

			DrawFPS(10, 10);

		EndDrawing();
	}

	CloseWindow();

	return 0;
}
