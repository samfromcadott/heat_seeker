#pragma once

namespace HSE {

struct Position {
	float x,y,z;

	Position() {
		x = 0.0;
		y = 0.0;
		z = 0.0;
	}

	Position(const glm::vec3&& v) {
		x = v.x;
		y = v.y;
		z = v.z;
	}

	operator glm::vec3() const {
		return glm::vec3(x,y,z);
	}

	Position(const Vector3&& v) {
		x = v.x;
		y = v.y;
		z = v.z;
	}

	operator Vector3() const {
		return Vector3{x,y,z};
	}
};

struct Rotation {
	float w,x,y,z;

	Rotation() {
		w = 1.0;
		x = 0.0;
		y = 0.0;
		z = 0.0;
	}

	Rotation(const glm::quat&& q) {
		w = q.w;
		x = q.x;
		y = q.y;
		z = q.z;
	}

	operator glm::quat() const {
		return glm::quat(w,x,y,z);
	}
};

}
