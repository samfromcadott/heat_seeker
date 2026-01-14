#pragma once

namespace HSE {

struct Position {
	float x,y,z;

	Position() {
		x = 0.0;
		y = 0.0;
		z = 0.0;
	}

	Position(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}

	Position(const vec3&& v) {
		x = v.x;
		y = v.y;
		z = v.z;
	}

	operator vec3() const {
		return vec3(x,y,z);
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

	Rotation(float w, float x, float y, float z) {
		this->w = w;
		this->x = x;
		this->y = y;
		this->z = z;
	}

	Rotation(const quat&& q) {
		w = q.w;
		x = q.x;
		y = q.y;
		z = q.z;
	}

	operator quat() const {
		return quat(w,x,y,z);
	}
};

struct Velocity {
	float x,y,z;

	Velocity() {
		x = 0.0;
		y = 0.0;
		z = 0.0;
	}

	Velocity(const vec3&& v) {
		x = v.x;
		y = v.y;
		z = v.z;
	}

	operator vec3() const {
		return vec3(x,y,z);
	}

	Velocity(const Vector3&& v) {
		x = v.x;
		y = v.y;
		z = v.z;
	}

	operator Vector3() const {
		return Vector3{x,y,z};
	}
};

inline vec3 raylib_to_glm(Vector3 v) {
	return vec3(v.x,v.y,v.z);
}

inline Vector3 glm_to_raylib(vec3 v) {
	return Vector3 {v.x,v.y,v.z};
}

}
