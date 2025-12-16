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
		world.system<Body&, Position&>().each(pos_to_body);
		world.system<Body&, Rotation&>().each(rot_to_body);
		world.system<Body&, Velocity&>().each(vel_to_body);
		world.system<CharacterBody&, Position&>().each(pos_to_character);
		world.system<CharacterBody&, Rotation&>().each(rot_to_character);
		world.system<CharacterBody&, Velocity&>().each(vel_to_character);
		world.system<PhysicsEngine&>().each(physics_update);
		world.system<CharacterBody&>().each(character_update);
		world.system<Body&, Position&>().each(body_to_pos);
		world.system<Body&, Rotation&>().each(body_to_rot);
		world.system<Body&, Velocity&>().each(body_to_vel);
		world.system<CharacterBody&, Position&>().each(character_to_pos);
		world.system<CharacterBody&, Rotation&>().each(character_to_rot);
		world.system<CharacterBody&, Velocity&>().each(character_to_vel);


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
		world.system().each(start_render);
		world.system().each(start_3D);
		world.system<HSE::Model&, HSE::Position&, HSE::Rotation&>().each(render_models);
		world.system().each(end_3D);
		world.system().each(end_render);
	}
};

struct CoreModule {
	CoreModule(flecs::world& world) {

	}
};

}
