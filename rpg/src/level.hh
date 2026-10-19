#pragma once

#include <heat_seeker.hh>
#include "components.hh"

void add_sensors(flecs::world world);
void use_parent(HSE::Entity entity, HSE::ContactAdded contact);
void slide_door(HSE::Entity entity, HSE::Position& position, HSE::Velocity& velocity, DoorSliding& ds);
void move_elevator(HSE::Entity entity, HSE::Position& position, HSE::Velocity& velocity, Elevator& lift);
