#include "Lighting.h"

#include <GLM\glm.hpp>

#include <iostream>

void Lighting::Directional(glm::vec3 eyeDirection, glm::vec3 lightDir, glm::vec3 vertexNormal, glm::vec4& fragColour) {

	//printf("%f, %f, %f\n", lightPos.x, lightPos.y, lightPos.z);

	eyeDirection = glm::normalize(eyeDirection);
	vertexNormal = glm::normalize(vertexNormal);
	//vertexNormal *= 10.0f;
	lightDir = glm::normalize(lightDir);

	float ambientStrength = 0.1f;
	float specularStrength = 0.8f;

	float diff = glm::max(glm::dot(vertexNormal, lightDir), 0.0f);

	glm::vec3 lightCol = glm::vec3(1.0f);

	glm::vec3 ambientCol = lightCol * ambientStrength;
	glm::vec3 diffuseCol = lightCol * diff;

	//printf("%f\n", diff);

	glm::vec3 viewDir = glm::normalize(eyeDirection - lightDir);
	glm::vec3 reflectDir = glm::reflect(lightDir, vertexNormal);

	float spec = glm::pow(glm::max(glm::dot(viewDir, reflectDir), 0.0f), 16.0f);
	glm::vec3 specularCol = specularStrength * spec * lightCol;

	glm::vec3 col = (ambientCol + diffuseCol + specularCol) * glm::vec3(fragColour);
	fragColour = glm::vec4(col, fragColour.a);
}