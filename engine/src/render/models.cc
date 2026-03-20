#include <heat_seeker.hh>

using namespace HSE;

HSE::Model::Model(const std::string& filename) {
	// Load the model if it hasn't been loaded yet
	if ( HSE::model_files.count(filename) == 0 ) {
		HSE::model_files[filename].model = LoadModel( filename.c_str() );
		HSE::model_files[filename].animations = LoadModelAnimations(filename.c_str(), &HSE::model_files[filename].animation_count);
	}

	// Set the shader for the model
	for (int i = 0; i < HSE::model_files[filename].model.materialCount; i++) {
		HSE::model_files[filename].model.materials[i].shader = gouraud_shader;
		HSE::model_files[filename].model.materials[i].maps[MATERIAL_MAP_DIFFUSE].texture = uv_debug_texture;
	}

	data = &HSE::model_files[filename];
}

void HSE::render_models(HSE::Model& m, HSE::Position& p, HSE::Rotation& r) {
	vec3 ax = axis( quat(r) );
	float ang = angle( quat(r) ) * 180.0 / PI;

	// Go to correct animation frame
	if (m.data->animation_count > 0)
		UpdateModelAnimation(m.data->model, m.data->animations[m.animation], m.frame);

	DrawModelEx(m.data->model, p, Vector3 {ax.x,ax.y,ax.z}, ang, {1.0,1.0,1.0}, m.debug_color);
}

void HSE::update_animation(HSE::Model& m) {
	m.frame++;
}
