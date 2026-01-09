#include <heat_seeker.hh>

#include "components.hh"
#include "systems.hh"

flecs::world HSE::Game;
std::map<std::string, HSE::ModelData> HSE::model_files;
Camera3D HSE::camera = {
	.position = Vector3 { 0.0f, 10.0f, 10.0f },
	.target = Vector3 { 0.0f, 0.0f, 0.0f },
	.up = Vector3 { 0.0f, 0.0f, 1.0f },
	.fovy = 45.0f,
	.projection = CAMERA_PERSPECTIVE,
};

using namespace HSE;

int main() {
	const int screenWidth = 1280;
	const int screenHeight = 720;

	InitWindow(screenWidth, screenHeight, "HEAT SEEKER");

	SetTargetFPS(60);
	DisableCursor();

	Game.import<HseCore>();

	Game.system<PlayerCamera&, Position&, Rotation&>().each(mouse_look);
	Game.system<Player, Position&, Rotation&, Velocity&, CharacterBody&, GroundMovement&>().each(player_movement);
	Game.system<Player, HSE::CharacterBody&, HSE::Velocity&>().each(player_jump);
	Game.system<Player, HSE::Position&, HSE::Rotation&>().each(shoot_ball);

	Game.component<Player>();

	Game.component<GroundMovement>()
		.member("direction", &GroundMovement::direction)
		.member("max_speed", &GroundMovement::max_speed)
		.member("acceleration", &GroundMovement::acceleration)
		.member("decceleration", &GroundMovement::decceleration)
		.member("acceleration_air", &GroundMovement::acceleration_air)
		.member("decceleration_air", &GroundMovement::decceleration_air);

	Game.component<PlayerCamera>()
		.member("rotation", &PlayerCamera::rotation)
		.member("offset", &PlayerCamera::offset);

	// Load models
	model_files["ball"].model = LoadModelFromMesh( GenMeshSphere(0.05, 4, 8) );
	model_files["plank"].model = LoadModelFromMesh( GenMeshCube(0.1, 2.0, 0.1) );
	model_files["can"].model = LoadModel("base/models/can.obj");
	model_files["floor"].model = LoadModelFromMesh( GenMeshCube(50.0, 50.0, 0.1) );

	// Create a couple entities
	flecs::entity can = Game.prefab();
	can.add<HSE::Model>();
	can.add<Position>();
	can.add<Rotation>();
	can.add<Body>();
	JPH::BodyCreationSettings can_body_settings(
		new JPH::RotatedTranslatedShape(
			JPH::RVec3::sZero(), JPH::Quat(0.7071068, 0, 0, 0.7071068),
			new JPH::CylinderShape(0.075, 0.05)
		),
		JPH::RVec3::sZero(),
		JPH::Quat::sIdentity(),
		JPH::EMotionType::Dynamic,
		Layers::MOVING
	);
	can.add<Velocity>();
	can.get_mut<HSE::Model>().data = &model_files["can"];

	flecs::entity plank = Game.entity();
	plank.add<HSE::Model>();
	plank.add<Position>();
	plank.add<Rotation>();
	plank.get_mut<Position>() = glm::vec3(0.0,0.0,0.0);
	plank.add<Body>();
	plank.set<Body>( Body(Game, JPH::BodyCreationSettings(
		new JPH::BoxShape( JPH::Vec3Arg(0.05, 1.0, 0.05) ),
		JPH::RVec3::sZero(),
		JPH::Quat::sIdentity(),
		JPH::EMotionType::Static,
		Layers::NON_MOVING
	)));
	plank.get_mut<HSE::Model>().data = &model_files["plank"];

	flecs::entity floor = Game.entity();
	floor.add<HSE::Model>();
	floor.add<Position>();
	floor.add<Rotation>();
	floor.get_mut<Position>() = glm::vec3(0.0,0.0,-1.0);
	floor.add<Body>();
	floor.set<Body>( Body(Game, JPH::BodyCreationSettings(
		new JPH::BoxShape( JPH::Vec3Arg(25.0, 25.0, 0.05) ),
		JPH::RVec3::sZero(),
		JPH::Quat::sIdentity(),
		JPH::EMotionType::Static,
		Layers::NON_MOVING
	)));
	floor.get_mut<HSE::Model>().data = &model_files["floor"];

	// Make the cans
	flecs::entity c1 = Game.entity().is_a(can);
	c1.set<Position>( glm::vec3(0,0,0.125) );
	c1.set<Body>( Body(Game, can_body_settings) );

	flecs::entity c2 = Game.entity().is_a(can);
	c2.set<Position>( glm::vec3(0,0.1,0.125) );
	c2.set<Body>( Body(Game, can_body_settings) );

	flecs::entity c3 = Game.entity().is_a(can);
	c3.set<Position>( glm::vec3(0,-0.1,0.125) );
	c3.set<Body>( Body(Game, can_body_settings) );

	flecs::entity c4 = Game.entity().is_a(can);
	c4.set<Position>( glm::vec3(0,0.05,0.275) );
	c4.set<Body>( Body(Game, can_body_settings) );

	flecs::entity c5 = Game.entity().is_a(can);
	c5.set<Position>( glm::vec3(0,-0.05,0.275) );
	c5.set<Body>( Body(Game, can_body_settings) );

	flecs::entity c6 = Game.entity().is_a(can);
	c6.set<Position>( glm::vec3(0,0,0.425) );
	c6.set<Body>( Body(Game, can_body_settings) );

	Game.script().filename("base/script/player.flecs").run();
	// Game.script().filename("base/script/can.flecs").run();
	// Game.script().filename("base/script/test.flecs").run();

	// Check to see if a player was added
	auto q = Game.query<Player>();

	std::cout << "Checking if a player was created...\n";
	q.each([](Player p) {
		std::cout << "This is a player.\n";
	});

	Game.import<flecs::stats>();

	// Creates REST server on default port (27750)
	Game.set<flecs::Rest>({});

	// Main game loop
	while ( !WindowShouldClose() ) {
		Game.progress();
	}

	CloseWindow();

	return 0;
}
