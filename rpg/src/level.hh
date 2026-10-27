#pragma once

#include <heat_seeker.hh>
#include "components.hh"

void add_sensors(flecs::world world);
void use_parent(HSE::Entity entity, HSE::ContactAdded contact);
void slide_door(HSE::Entity entity, HSE::Position& position, HSE::Velocity& velocity, DoorSliding& ds);
void close_door_sliding(HSE::Entity entity);
void move_elevator(HSE::Entity entity, HSE::Position& position, HSE::Velocity& velocity, Elevator& lift);
void slide_platform(HSE::Entity entity, HSE::Position& position, HSE::Velocity& velocity, PlatSliding& ps);
void rotate_platform(HSE::Entity entity, HSE::Body& body, PlatRotating& pr);
