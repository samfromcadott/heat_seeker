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
		// return *this;
	}

	operator glm::vec3() const {
		return glm::vec3(x,y,z);
	}
};

};
