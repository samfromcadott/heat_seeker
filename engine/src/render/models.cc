#include <heat_seeker.hh>

void HSE::render_models(HSE::Model& m, HSE::Position& p, HSE::Rotation& r) {
	glm::vec3 axis = glm::axis( glm::quat(r) );
	float angle = glm::angle( glm::quat(r) ) * 180.0 / PI;
	DrawModelWiresEx(m.data->model, p, Vector3 {axis.x,axis.y,axis.z}, angle, {1.0,1.0,1.0}, BLACK);
}
