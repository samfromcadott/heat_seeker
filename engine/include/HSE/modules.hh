#pragma once

// struct HsePhysics {
// 	HsePhysics(flecs::world& world);
// };
//
// struct HseRender {
// 	HseRender(flecs::world& world);
// };
//
// struct HseCore {
// 	HseCore(flecs::world& world);
// };

namespace HSE {

void init_core(flecs::world& world);
void init_physics(flecs::world& world);
void init_render(flecs::world& world);

}
