#ifndef _LIGHTING_H_
#define _LIGHTING_H_

#include <GLM\glm.hpp>

/*
Lighting has one function currently and that is the directional lighting function.
*/
namespace Lighting {
	void Directional(glm::vec3 eyeDirection, glm::vec3 lightNormal, glm::vec3 vertexNormal, glm::vec3 fragPos, glm::vec4& fragColour);
}

#endif