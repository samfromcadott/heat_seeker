#pragma once

struct PhysicsModule {
	PhysicsModule(flecs::world& world) {
		// Add physics engine
		world.component<HSE::PhysicsEngine>().add(flecs::Singleton);
		world.set<HSE::PhysicsEngine>( HSE::PhysicsEngine() );
		world.get_mut<HSE::PhysicsEngine>().set_world(world);

		world.component<HSE::Body>();

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
	}
};

struct RenderModule {
	RenderModule(flecs::world& world) {
		world.system().kind(flecs::PostUpdate).each(HSE::start_render);
		world.system().kind(flecs::PostUpdate).each(HSE::start_3D);
		world.system<HSE::Model&, HSE::Position&, HSE::Rotation&>().kind(flecs::PostUpdate).each(HSE::render_models);
		world.system().kind(flecs::PostUpdate).each(HSE::end_3D);
		world.system().kind(flecs::PostUpdate).each(HSE::end_render);
	}
};

struct CoreModule {
	CoreModule(flecs::world& world) {
		// Register basic types
		world.component<glm::vec2>("vec2")
		.member("x", &glm::vec2::x)
		.member("y", &glm::vec2::y);

		world.component<glm::vec3>("vec3")
		.member("x", &glm::vec3::x)
		.member("y", &glm::vec3::y)
		.member("z", &glm::vec3::z);

		// world.component<HSE::Position>()
		// .member("x", &HSE::Position::x)
		// .member("y", &HSE::Position::y)
		// .member("z", &HSE::Position::z);
  //
		// world.component<HSE::Rotation>()
		// .member("w", &HSE::Rotation::w)
		// .member("x", &HSE::Rotation::x)
		// .member("y", &HSE::Rotation::y)
		// .member("z", &HSE::Rotation::z);

		world.component<HSE::Velocity>("Velocity")
		.member("x", &HSE::Velocity::x)
		.member("y", &HSE::Velocity::y)
		.member("z", &HSE::Velocity::z);

		world.import<PhysicsModule>();
		world.import<RenderModule>();
	}
};

