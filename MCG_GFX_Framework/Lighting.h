#pragma once

#include <GLM\glm.hpp>

namespace Lighting {
	void Directional(glm::vec3 eyeDirection, glm::vec3 lightNormal, glm::vec3 vertexNormal, glm::vec4& fragColour);
}

