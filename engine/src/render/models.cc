#include <heat_seeker.hh>

using namespace HSE;

HSE::Model::Model(const std::string& filename) {
	if ( HSE::model_files.count(filename) == 0 ) HSE::model_files[filename].model = LoadModel( filename.c_str() );
	// Set the shader for the model
	for (int i = 0; i < HSE::model_files[filename].model.materialCount; i++) {
		HSE::model_files[filename].model.materials[i].shader = GouraudShader;
	}
	data = &HSE::model_files[filename];
}

void HSE::render_models(HSE::Model& m, HSE::Position& p, HSE::Rotation& r) {
	vec3 ax = axis( quat(r) );
	float ang = angle( quat(r) ) * 180.0 / PI;
	DrawModelEx(m.data->model, p, Vector3 {ax.x,ax.y,ax.z}, ang, {1.0,1.0,1.0}, m.debug_color);
}
