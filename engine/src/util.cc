#include <heat_seeker.hh>

using namespace HSE;

vec3 HSE::move_toward(const vec3& from, const vec3& to, float delta) {
	auto vd = to - from;
	return length(vd) <= delta? to : from + normalize(vd) * delta;
}
