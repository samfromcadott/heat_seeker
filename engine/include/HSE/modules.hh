#pragma once

struct HsePhysics {
	HsePhysics(flecs::world& world) {
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
			.member("max_strength", &HSE::CharacterBodyOptions::max_strength);

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

		world.observer<HSE::CharacterBody>().event(flecs::OnSet).each([](flecs::entity e, HSE::CharacterBody& b) {
			b.set_owner(e);
		});

		world.observer<HSE::CharacterBody>().event(flecs::OnSet).each([](flecs::entity e, HSE::CharacterBody& b) {
			b.set_owner(e);
		});

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
};

struct HseRender {
	HseRender(flecs::world& world) {
		world.system().kind(flecs::PostUpdate).each(HSE::start_render);
		world.system().kind(flecs::PostUpdate).each(HSE::start_3D);
		world.system<HSE::Model&, HSE::Position&, HSE::Rotation&>().kind(flecs::PostUpdate).each(HSE::render_models);
		world.system().kind(flecs::PostUpdate).each(HSE::end_3D);
		world.system().kind(flecs::PostUpdate).each(HSE::end_render);

		world.component<HSE::Model>();
		// ecs_function_desc_t load_model_desc;
		// load_model_desc.name = "load_model";
		// load_model_desc.return_type = world.id<HSE::Model>();
		// load_model_desc.params[0] = { .name = "filename", .type = ecs_id(ecs_string_t) };
		// load_model_desc.callback = HSE::load_model;
		// ecs_function_init(world, &load_model_desc);
		ecs_function_desc_t load_model_desc = {
			.name = "load_model",
			.params = {{ .name = "filename", .type = ecs_id(ecs_string_t) }},
			.return_type = world.id<HSE::Model>(),
			.callback = HSE::load_model,
		};
		ecs_function_init(world, &load_model_desc);
	}
};

struct HseCore {
	HseCore(flecs::world& world) {
		// Register basic types
		world.component<glm::vec2>()
			.member("x", &glm::vec2::x)
			.member("y", &glm::vec2::y);

		world.component<glm::vec3>()
			.member("x", &glm::vec3::x)
			.member("y", &glm::vec3::y)
			.member("z", &glm::vec3::z);

		world.component<glm::quat>()
			.member("w", &glm::quat::w)
			.member("x", &glm::quat::x)
			.member("y", &glm::quat::y)
			.member("z", &glm::quat::z);

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
};
