#include <heat_seeker.hh>

using namespace HSE;

ModelData::ModelData(const std::string& filename) {
	model = LoadModel( filename.c_str() );
	animations = LoadModelAnimations(filename.c_str(), &animation_count);

	// Populate anim_names
	for (int i = 0; i < animation_count; i++) {
		anim_names[animations[i].name] = i;
	}
}

HSE::Model::Model(const std::string& filename) {
	if ( HSE::model_files.count(filename) == 0 )
		HSE::model_files[filename] = ModelData(filename);

	// Set the shader for the model
	for (int i = 0; i < HSE::model_files[filename].model.materialCount; i++) {
		HSE::model_files[filename].model.materials[i].shader = gouraud_shader;
	}

	data = &HSE::model_files[filename];
}

void HSE::Model::play(const std::string& anim_name) {
	if (data->anim_names.count(anim_name) == 0) return; // Check if anim_name exists
	if (data->anim_names[anim_name] == animation) return; // Check if animation is already playing

	animation = data->anim_names[anim_name];
	frame = 0;
}

void HSE::render_models(HSE::Model& m, HSE::Position& p, HSE::Rotation& r) {
	vec3 ax = axis( quat(r) );
	float ang = angle( quat(r) ) * 180.0 / PI;

	// Go to correct animation frame
	if (m.data->animation_count > 0)
		UpdateModelAnimation(m.data->model, m.data->animations[m.animation], m.frame);

	DrawModelEx(m.data->model, p, Vector3 {ax.x,ax.y,ax.z}, ang, {1.0,1.0,1.0}, WHITE);
}

void HSE::update_animation(HSE::Model& m) {
	m.frame++;
}
