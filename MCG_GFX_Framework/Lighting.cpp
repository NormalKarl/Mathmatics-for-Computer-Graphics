#include "Lighting.h"

#include <GLM\glm.hpp>

void Lighting::Directional(glm::vec3 eyeDirection, glm::vec3 lightDir, glm::vec3 vertexNormal, glm::vec4& fragColour) {

	//printf("%f, %f, %f\n", lightPos.x, lightPos.y, lightPos.z);

	glm::vec3 objectColour = glm::vec3(0.1f, 0.5f, 0.5f);

	vertexNormal = glm::normalize(vertexNormal);
	lightDir = glm::normalize(lightDir);


	float diff = glm::max(glm::dot(vertexNormal, lightDir), 0.0f);

	glm::vec3 lightCol = glm::vec3(1.0f);

	glm::vec3 ambientCol = lightCol * 0.1f;
	glm::vec3 diffuseCol = lightCol * diff;


	glm::vec3 viewDir = glm::normalize(eyeDirection - lightDir);
	glm::vec3 reflectDir = glm::reflect(lightDir, vertexNormal);

	float specularStrength = 0.5;
	float spec = glm::pow(glm::max(glm::dot(viewDir, reflectDir), 0.0f), 16.0f);
	glm::vec3 specular = specularStrength * spec * lightCol;

	glm::vec3 col = (ambientCol + diffuseCol + specular) * objectColour;
	fragColour = glm::vec4(col, 1.0f);
}