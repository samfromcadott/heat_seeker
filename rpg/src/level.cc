#include "level.hh"

using namespace HSE;

void add_sensors(flecs::world world) {
	flecs::query<> find_sliding_doors = world.query_builder<>()
	.with<Body>()
	.with<DoorSliding>()
	.build();

	ecs_defer_begin(world);
	find_sliding_doors.each([](Entity entity) {
		auto& ds = get<DoorSliding>(entity);
		ds.start= get<Position>(entity);
		get<Body>(entity).set_motion_type(JPH::EMotionType::Kinematic);
		entity.add<Velocity>();

		// Get bounds of the door
		auto [min, max] = get<Body>(entity).get_bounds();

		// Make a child object
		auto sensor = entity.child();

		BodyOptions options;
		options.motion_type = JPH::EMotionType::Static;
		options.sensor = true;
		options.layer = 4;
		options.mask = 2;
		options.shape.type = BOX;
		options.shape.size = max - min + vec3(4,4,4);

		sensor.set<BodyOptions>(options);
		sensor.add<UseParent>();
		sensor.set<Position>( get<Position>(entity) );
	});
	ecs_defer_end(world);

	flecs::query<> find_elevators = world.query_builder<>()
	.with<Body>()
	.with<Elevator>()
	.build();

	ecs_defer_begin(world);
	find_elevators.each([](Entity entity) {
		auto& lift = get<Elevator>(entity);
		lift.start= get<Position>(entity);
		get<Body>(entity).set_motion_type(JPH::EMotionType::Kinematic);
		entity.add<Velocity>();

		// Get bounds of the door
		auto [min, max] = get<Body>(entity).get_bounds();

		// Make a child object
		auto sensor = entity.child();

		BodyOptions options;
		options.motion_type = JPH::EMotionType::Static;
		options.sensor = true;
		options.layer = 4;
		options.mask = 2;
		options.shape.type = BOX;
		options.shape.size = max - min + vec3(0,0, lift.distance + 2.0);
		options.shape.translation.z = options.shape.size.z / 2.0;

		sensor.set<BodyOptions>(options);
		sensor.add<UseParent>();
		sensor.set<Position>( get<Position>(entity) );
	});
	ecs_defer_end(world);

	flecs::query<> find_rotating = world.query_builder<>()
	.with<Body>()
	.with<PlatRotating>()
	.build();

	ecs_defer_begin(world);
	find_rotating.each([](Entity entity) {
		get<Body>(entity).set_motion_type(JPH::EMotionType::Kinematic);
		entity.add<Velocity>();
	});
	ecs_defer_end(world);

	flecs::query<> find_sliding_plat = world.query_builder<>()
	.with<Body>()
	.with<PlatSliding>()
	.build();

	ecs_defer_begin(world);
	find_sliding_plat.each([](Entity entity) {
		auto& ps = get<PlatSliding>(entity);
		ps.start = get<Position>(entity);
		get<Body>(entity).set_motion_type(JPH::EMotionType::Kinematic);
		entity.add<Velocity>();
	});
	ecs_defer_end(world);
}

void use_parent(HSE::Entity entity, HSE::ContactAdded contact) {
	// Only players and monsters can open doors
	if ( not has<Player>(contact.other) and not has<Monster>(contact.other) )
		return;

	auto parent = entity.parent();

	if ( auto ds = get_if<DoorSliding>(parent) ) {
		if (ds->state == CLOSED) ds->state = OPENING;
	}

	else if ( auto lift = get_if<Elevator>(parent) ) {
		if (lift->state == A) lift->state = A_TO_B;
		else if (lift->state == B) lift->state = B_TO_A;
	}
}

void slide_door(HSE::Entity entity, Position& position, Velocity& velocity, DoorSliding& ds) {
	if (ds.state == OPEN or ds.state == CLOSED) return;

	velocity = ds.state == OPENING? ds.speed * ds.direction : ds.speed * -ds.direction;

	vec3 goal = ds.state == OPENING? ds.start + ds.direction * ds.distance : ds.start;
	vec3 next_pos = vec3(position) + vec3(velocity) * GetFrameTime();

	// Stop if close to goal
	if ( distance(goal, next_pos) < 0.1 ) {
		ds.state = ds.state == OPENING? OPEN : CLOSED;
		position = goal;
		velocity = vec3(0,0,0);
	}

	if (ds.state == OPEN)
		start_timer(entity, "close_door_sliding", 5.0);
}

void close_door_sliding(HSE::Entity entity) {
	get<DoorSliding>(entity).state = CLOSING;
}

void move_elevator(HSE::Entity entity, Position& position, Velocity& velocity, Elevator& lift) {
	if (lift.state == A or lift.state == B) return;

	const vec3 up(0,0,1);

	velocity = lift.state == A_TO_B? lift.speed * up : lift.speed * -up;

	vec3 goal = lift.state == A_TO_B? lift.start + up * lift.distance : lift.start;
	vec3 next_pos = vec3(position) + vec3(velocity) * GetFrameTime();

	// Stop if close to goal
	if ( distance(goal, next_pos) < 0.1 ) {
		lift.state = lift.state == A_TO_B? B : A;
		position = goal;
		velocity = vec3(0,0,0);
	}
}

void slide_platform(HSE::Entity entity, Position& position, Velocity& velocity, PlatSliding& ps) {
	if (not ps.moving) return;

	velocity = ps.state == A_TO_B? ps.speed * ps.direction : ps.speed * -ps.direction;

	vec3 goal = ps.state == A_TO_B? ps.start + ps.direction * ps.distance : ps.start;
	vec3 next_pos = vec3(position) + vec3(velocity) * GetFrameTime();

	// Stop if close to goal
	if ( distance(goal, next_pos) < 0.1 ) {
		ps.state = ps.state == A_TO_B? B_TO_A : A_TO_B;
		velocity = -vec3(velocity);
	}
}

void rotate_platform(HSE::Entity entity, Body& body, PlatRotating& pr) {
	if (pr.moving) {
		body.set_angular_velocity(pr.axis * pr.speed * 0.017f);
	}
}
