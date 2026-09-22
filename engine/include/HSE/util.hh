#pragma once

namespace HSE {

template <typename T>
T move_toward(const T from, const T to, const T delta) {
	return abs(to - from) <= delta ? to : from + SIGN(to - from) * delta;
}

vec3 move_toward(const vec3& from, const vec3& to, float delta);

}
