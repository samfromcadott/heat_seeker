#include <heat_seeker.hh>

using namespace HSE;

const char* gouraud_vert =
#include "render/shaders/gouraud_vert.glsl"
;

const char* gouraud_frag =
#include "render/shaders/gouraud_frag.glsl"
;

HsePhysics::HsePhysics(flecs::world& world) {
	// Add physics engine
	world.component<HSE::PhysicsEngine>().add(flecs::Singleton);
	world.set<HSE::PhysicsEngine>( HSE::PhysicsEngine() );
	world.get_mut<HSE::PhysicsEngine>().set_world(world);

	// Add components
	world.component<HSE::Body>();
	world.component<HSE::CharacterBody>();

	world.component<HSE::ShapeType>()
		.constant("NONE", HSE::NONE)
		.constant("SPHERE", HSE::SPHERE)
		.constant("BOX", HSE::BOX)
		.constant("CYLINDER", HSE::CYLINDER)
		.constant("CAPSULE", HSE::CAPSULE);

	world.component<JPH::EMotionType>()
		.constant("Static", JPH::EMotionType::Static)
		.constant("Dynamic", JPH::EMotionType::Dynamic)
		.constant("Kinematic", JPH::EMotionType::Kinematic);

	world.component<HSE::ShapeOptions>()
		.member("type", &HSE::ShapeOptions::type)
		.member("translation", &HSE::ShapeOptions::translation)
		.member("rotation", &HSE::ShapeOptions::rotation)
		.member("height", &HSE::ShapeOptions::height)
		.member("radius", &HSE::ShapeOptions::radius)
		.member("size", &HSE::ShapeOptions::size);

	world.component<HSE::BodyOptions>()
		.member("shape", &HSE::BodyOptions::shape)
		.member("motion_type", &HSE::BodyOptions::motion_type)
		.member("object_layer", &HSE::BodyOptions::object_layer);

	world.component<HSE::CharacterBodyOptions>()
		.member("shape", &HSE::CharacterBodyOptions::shape)
		.member("up", &HSE::CharacterBodyOptions::up)
		.member("max_slope", &HSE::CharacterBodyOptions::max_slope)
		.member("mass", &HSE::CharacterBodyOptions::mass)
		.member("max_strength", &HSE::CharacterBodyOptions::max_strength)
		.member("gravity_scale", &HSE::CharacterBodyOptions::gravity_scale);

	// Register systems
	world.system<HSE::Body&, HSE::Position&>().kind(flecs::PreUpdate).each(HSE::pos_to_body);
	world.system<HSE::Body&, HSE::Rotation&>().kind(flecs::PreUpdate).each(HSE::rot_to_body);
	world.system<HSE::Body&, HSE::Velocity&>().kind(flecs::PreUpdate).each(HSE::vel_to_body);
	world.system<HSE::CharacterBody&, HSE::Position&>().kind(flecs::PreUpdate).each(HSE::pos_to_character);
	world.system<HSE::CharacterBody&, HSE::Rotation&>().kind(flecs::PreUpdate).each(HSE::rot_to_character);
	world.system<HSE::CharacterBody&, HSE::Velocity&>().kind(flecs::PreUpdate).each(HSE::vel_to_character);
	world.system<HSE::PhysicsEngine&>().kind(flecs::PreUpdate).each(HSE::physics_update);
	world.system<HSE::CharacterBody&>().kind(flecs::PreUpdate).each(HSE::character_update);
	world.system<HSE::Body&, HSE::Position&>().kind(flecs::PreUpdate).each(HSE::body_to_pos);
	world.system<HSE::Body&, HSE::Rotation&>().kind(flecs::PreUpdate).each(HSE::body_to_rot);
	world.system<HSE::Body&, HSE::Velocity&>().kind(flecs::PreUpdate).each(HSE::body_to_vel);
	world.system<HSE::CharacterBody&, HSE::Position&>().kind(flecs::PreUpdate).each(HSE::character_to_pos);
	world.system<HSE::CharacterBody&, HSE::Rotation&>().kind(flecs::PreUpdate).each(HSE::character_to_rot);
	world.system<HSE::CharacterBody&, HSE::Velocity&>().kind(flecs::PreUpdate).each(HSE::character_to_vel);


	// Set body observer
	world.observer<HSE::Body>().event(flecs::OnSet).each([](flecs::entity e, HSE::Body& b) {
		b.set_owner(e);
	});

	world.observer<HSE::Body>().event(flecs::OnRemove).each([](flecs::entity e, HSE::Body& b) {
		b.destroy();
	});

	world.observer<HSE::CharacterBody>().event(flecs::OnSet).each([](flecs::entity e, HSE::CharacterBody& b) {
		b.set_owner(e);
	});

	// world.observer<HSE::CharacterBody>().event(flecs::OnSet).each([](flecs::entity e, HSE::CharacterBody& b) {
	// 	b.set_owner(e);
	// });

	world.observer<HSE::BodyOptions>().event(flecs::OnSet).each([&](flecs::entity e, HSE::BodyOptions& o) {
		// Add a new body using the options
		e.set<HSE::Body>( HSE::Body(world, o) );
		e.remove<HSE::BodyOptions>();
	});

	world.observer<HSE::CharacterBodyOptions>().event(flecs::OnSet).each([&](flecs::entity e, HSE::CharacterBodyOptions& o) {
		// Add a new character body using the options
		e.set<HSE::CharacterBody>( HSE::CharacterBody(world, o) );
		e.remove<HSE::CharacterBodyOptions>();
	});
}



HseRender::HseRender(flecs::world& world) {
	world.system().kind(flecs::PostUpdate).each(HSE::start_render);
	world.system().kind(flecs::PostUpdate).each(HSE::start_3D);
	world.system<HSE::Model&, HSE::Position&, HSE::Rotation&>().kind(flecs::PostUpdate).each(HSE::render_models);
	world.system().kind(flecs::PostUpdate).each(HSE::end_3D);
	world.system().kind(flecs::PostUpdate).each(HSE::update_ui);
	world.system().kind(flecs::PostUpdate).each(HSE::end_render);

	world.component<HSE::Model>();

	ecs_function_desc_t load_model_desc = {
		.name = "load_model",
		.params = {{ .name = "filename", .type = ecs_id(ecs_string_t) }},
		.return_type = world.id<HSE::Model>(),
		.callback = HSE::load_model,
	};

	ecs_function_init(world, &load_model_desc);

	world.observer<HSE::Model>().event(flecs::OnSet).each([&](flecs::entity e, HSE::Model& m){
		m.debug_color = ColorFromHSV(rand()%360, (float)rand()/(float)RAND_MAX, 1.0);
	});

	gouraud_shader =  LoadShaderFromMemory(gouraud_vert, gouraud_frag);
}


HseCore::HseCore(flecs::world& world) {
	// Register basic types
	world.component<HSE::vec2>()
		.member("x", &HSE::vec2::x)
		.member("y", &HSE::vec2::y);

	world.component<HSE::vec3>()
		.member("x", &HSE::vec3::x)
		.member("y", &HSE::vec3::y)
		.member("z", &HSE::vec3::z);

	world.component<HSE::quat>()
		.member("w", &HSE::quat::w)
		.member("x", &HSE::quat::x)
		.member("y", &HSE::quat::y)
		.member("z", &HSE::quat::z);

	world.component<HSE::Position>()
		.member("x", &HSE::Position::x)
		.member("y", &HSE::Position::y)
		.member("z", &HSE::Position::z);

	world.component<HSE::Rotation>()
		.member("w", &HSE::Rotation::w)
		.member("x", &HSE::Rotation::x)
		.member("y", &HSE::Rotation::y)
		.member("z", &HSE::Rotation::z);

	world.component<HSE::Velocity>()
		.member("x", &HSE::Velocity::x)
		.member("y", &HSE::Velocity::y)
		.member("z", &HSE::Velocity::z);

	world.import<HsePhysics>();
	world.import<HseRender>();
}

