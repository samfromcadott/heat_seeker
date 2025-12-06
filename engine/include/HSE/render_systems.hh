#pragma once

namespace HSE {

struct Position;

void start_render(); ///< Begins rendering
void start_3D(); ///< Begins rendering from the active 3D camera
void end_render(); ///< Ends rendering
void end_3D(); ///< Ends rendering from the active 3D camera
void render_models(HSE::Model m, Position p, Rotation r); ///< Renders non-BVH models
void render_bvh_models(); ///< Renders BVH models
void update_armature(); ///< Advances the animation armatures of models

}
