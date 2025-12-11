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

	Game.import<PhysicsModule>();

	Game.system<Player, Position&, Rotation&>().each(mouse_look);
	Game.system<Player, HSE::Position&, HSE::Rotation&>().each(shoot_ball);

	Game.import<RenderModule>();

	// Load models
	model_files["ball"].model = LoadModelFromMesh( GenMeshSphere(0.05, 4, 8) );
	model_files["floor"].model = LoadModelFromMesh( GenMeshCube(0.1, 2.0, 0.1) );
	model_files["can"].model = LoadModel("rpg/base/models/can.obj");

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

	flecs::entity floor = Game.entity();
	floor.add<HSE::Model>();
	floor.add<Position>();
	floor.add<Rotation>();
	floor.get_mut<Position>() = glm::vec3(0.0,0.0,0.0);
	floor.add<Body>();
	floor.set<Body>( Body(Game.get_mut<PhysicsEngine>(), JPH::BodyCreationSettings(
		new JPH::BoxShape( JPH::Vec3Arg(0.05, 1.0, 0.05) ),
		JPH::RVec3::sZero(),
		JPH::Quat::sIdentity(),
		JPH::EMotionType::Static,
		Layers::NON_MOVING
	)));

	// Make the player
	flecs::entity player = Game.entity();
	player.add<Player>();
	player.add<Position>();
	player.add<Rotation>();
	player.get_mut<Position>() = glm::vec3(-2.0,0,0.5);
	floor.get_mut<HSE::Model>().data = &model_files["floor"];

	// Make the cans
	flecs::entity c1 = Game.entity().is_a(can);
	c1.set<Position>( glm::vec3(0,0,0.125) );
	c1.set<Body>( Body(Game.get_mut<PhysicsEngine>(), can_body_settings) );

	flecs::entity c2 = Game.entity().is_a(can);
	c2.set<Position>( glm::vec3(0,0.1,0.125) );
	c2.set<Body>( Body(Game.get_mut<PhysicsEngine>(), can_body_settings) );

	flecs::entity c3 = Game.entity().is_a(can);
	c3.set<Position>( glm::vec3(0,-0.1,0.125) );
	c3.set<Body>( Body(Game.get_mut<PhysicsEngine>(), can_body_settings) );

	flecs::entity c4 = Game.entity().is_a(can);
	c4.set<Position>( glm::vec3(0,0.05,0.275) );
	c4.set<Body>( Body(Game.get_mut<PhysicsEngine>(), can_body_settings) );

	flecs::entity c5 = Game.entity().is_a(can);
	c5.set<Position>( glm::vec3(0,-0.05,0.275) );
	c5.set<Body>( Body(Game.get_mut<PhysicsEngine>(), can_body_settings) );

	flecs::entity c6 = Game.entity().is_a(can);
	c6.set<Position>( glm::vec3(0,0,0.425) );
	c6.set<Body>( Body(Game.get_mut<PhysicsEngine>(), can_body_settings) );

	Game.observer<ContactAdded>()
		.event(flecs::OnSet)
		.each([](flecs::entity e, ContactAdded& c) {
			if ( e.get<HSE::Model>().data != &model_files["ball"] ) return;

			std::cout << "ball collided with ";

			if ( c.other.get<HSE::Model>().data == &model_files["floor"] ) std::cout << "floor" << '\n';
			else if ( c.other.get<HSE::Model>().data == &model_files["can"] ) std::cout << "can" << '\n';
	});

	// Main game loop
	while ( !WindowShouldClose() ) {
		Game.progress();
	}

	CloseWindow();

	return 0;
}
