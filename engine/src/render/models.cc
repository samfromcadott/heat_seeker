#include <heat_seeker.hh>

HSE::Model::Model(const std::string& filename) {
	if ( HSE::model_files.count(filename) == 0 ) HSE::model_files[filename].model = LoadModel( filename.c_str() );
	data = &HSE::model_files[filename];
}

void HSE::render_models(HSE::Model& m, HSE::Position& p, HSE::Rotation& r) {
	glm::vec3 axis = glm::axis( glm::quat(r) );
	float angle = glm::angle( glm::quat(r) ) * 180.0 / PI;
	DrawModelWiresEx(m.data->model, p, Vector3 {axis.x,axis.y,axis.z}, angle, {1.0,1.0,1.0}, BLACK);
}
