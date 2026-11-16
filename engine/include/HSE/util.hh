#pragma once

namespace HSE {

template <typename T>
T sign(T val) {
	return (T(0) < val) - (val < T(0));
}

template <typename T>
T move_toward(const T from, const T to, const T delta) {
	return abs(to - from) <= delta ? to : from + sign(to - from) * delta;
}

template <typename T>
T rotate_toward(T from, T to, T delta) {
	T diff = fmod(to - from, 2*PI);
	diff = fmod(2*diff, 2*PI) - diff;
	T abs_diff = abs(diff);
	return from + std::clamp(delta, abs_diff - PI, abs_diff) * (diff >= 0.0 ? 1.0 : -1.0);
}

vec3 move_toward(const vec3& from, const vec3& to, float delta);

}
