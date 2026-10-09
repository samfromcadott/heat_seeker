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
	data = Asset<ModelData>(filename);
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

	update_lights(m.data->model.materials[0].shader, p);

	DrawModelEx(m.data->model, p, Vector3 {ax.x,ax.y,ax.z}, ang, {1.0,1.0,1.0}, WHITE);
}

void HSE::update_animation(HSE::Model& m) {
	m.frame++;
}

void HSE::update_lights(Shader& shader, Position& p) {
	// Query lights in the scene
	flecs::query<Light&, Position&> q =  Game.query<Light&, Position&>();
	int i = 0;

	q.each([&](Light& light, Position& position) {
		// For each light set its corresponding value in the shader
		int loc_power = GetShaderLocation( shader, TextFormat("lights[%i].power", i) );
		int loc_pos = GetShaderLocation( shader, TextFormat("lights[%i].position", i) );;
		int loc_color = GetShaderLocation( shader, TextFormat("lights[%i].color", i) );;

		SetShaderValue(shader, loc_power, &light.power, SHADER_UNIFORM_FLOAT);
		SetShaderValue(shader, loc_pos, &position, SHADER_UNIFORM_VEC3);
		SetShaderValue(shader, loc_color, &light.color, SHADER_UNIFORM_VEC4);

		i++;
	});
}
