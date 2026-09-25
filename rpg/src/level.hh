#pragma once

#include <heat_seeker.hh>
#include "components.hh"

void use_parent(HSE::Entity entity, HSE::ContactAdded contact);
void slide_door(HSE::Entity entity, HSE::Position& position, HSE::Velocity& velocity, DoorSliding& ds);
