#include <heat_seeker.hh>

void HSE::init(const std::string& title, const int width, const int height) {
	InitWindow( width, height, title.c_str() );
	InitAudioDevice();
	SetTargetFPS(60);
	File::init();
}

void HSE::quit() {
	Game.quit();
	UnloadShader(gouraud_shader);
	CloseWindow();
}

void HSE::pause(flecs::world& world) {
	std::cout << "pause\n";
	flecs::query<> q  = world.query_builder<>()
	.with(flecs::System)
	.without<NoPause>()
	.build();

	ecs_defer_begin(world);
	q.each([](flecs::entity system){
		system.disable();
	});
	ecs_defer_end(world);
}

void HSE::resume(flecs::world& world) {
	std::cout << "resume\n";
	flecs::query<> q  = world.query_builder<>()
	.with(flecs::System)
	.with(flecs::Disabled)
	.without<NoPause>()
	.build();

	ecs_defer_begin(world);
	q.each([](flecs::entity system){
		system.enable();
	});
	ecs_defer_end(world);
}
