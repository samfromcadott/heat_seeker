#pragma once
namespace HSE {

struct PhysicsModule {
	PhysicsModule(flecs::world& world) {
		// Add physics engine
		world.component<PhysicsEngine>().add(flecs::Singleton);
		world.set<PhysicsEngine>( PhysicsEngine() );
		world.get_mut<PhysicsEngine>().set_world(world);

		world.component<Body>();

		// Register systems
		world.system<Body&, Position&>().kind(flecs::PreUpdate).each(pos_to_body);
		world.system<Body&, Rotation&>().kind(flecs::PreUpdate).each(rot_to_body);
		world.system<Body&, Velocity&>().kind(flecs::PreUpdate).each(vel_to_body);
		world.system<CharacterBody&, Position&>().kind(flecs::PreUpdate).each(pos_to_character);
		world.system<CharacterBody&, Rotation&>().kind(flecs::PreUpdate).each(rot_to_character);
		world.system<CharacterBody&, Velocity&>().kind(flecs::PreUpdate).each(vel_to_character);
		world.system<PhysicsEngine&>().kind(flecs::PreUpdate).each(physics_update);
		world.system<CharacterBody&>().kind(flecs::PreUpdate).each(character_update);
		world.system<Body&, Position&>().kind(flecs::PreUpdate).each(body_to_pos);
		world.system<Body&, Rotation&>().kind(flecs::PreUpdate).each(body_to_rot);
		world.system<Body&, Velocity&>().kind(flecs::PreUpdate).each(body_to_vel);
		world.system<CharacterBody&, Position&>().kind(flecs::PreUpdate).each(character_to_pos);
		world.system<CharacterBody&, Rotation&>().kind(flecs::PreUpdate).each(character_to_rot);
		world.system<CharacterBody&, Velocity&>().kind(flecs::PreUpdate).each(character_to_vel);


		// Set body observer
		world.observer<Body>().event(flecs::OnSet).each([](flecs::entity e, Body& b) {
			b.set_owner(e);
		});

		world.observer<CharacterBody>().event(flecs::OnSet).each([](flecs::entity e, CharacterBody& b) {
			b.set_owner(e);
		});
	}
};

struct RenderModule {
	RenderModule(flecs::world& world) {
		world.system().kind(flecs::PostUpdate).each(start_render);
		world.system().kind(flecs::PostUpdate).each(start_3D);
		world.system<HSE::Model&, HSE::Position&, HSE::Rotation&>().kind(flecs::PostUpdate).each(render_models);
		world.system().kind(flecs::PostUpdate).each(end_3D);
		world.system().kind(flecs::PostUpdate).each(end_render);
	}
};

struct CoreModule {
	CoreModule(flecs::world& world) {

	}
};

}
