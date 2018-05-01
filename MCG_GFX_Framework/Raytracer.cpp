#include "Raytracer.h"
#include "Surface.h"
#include "Geometry.h"

#include "MCG_GFX_Lib.h"
#include <GLM\gtc\matrix_transform.hpp>
#include <GLM/glm.hpp>
#include <SDL\SDL.h>

#include <cstdio>

bool Sphere::intersect(const Ray& _ray, glm::vec3& _normal) {
	glm::vec3 pos = m_position - _ray.origin;

	float dot = glm::dot(pos, _ray.direction);

	glm::vec3 distance = m_position - _ray.origin - (dot * _ray.direction);

	float mag = glm::sqrt(distance.x * distance.x + distance.y * distance.y);

	if (mag <= m_radius)
	{
		float x = glm::sqrt(m_radius * m_radius - mag * mag);
		glm::vec3 near = _ray.origin + ((dot - x) * _ray.direction);

		if (near.z > 0)
			return false;

		_normal = near - m_position;
		return true;
	}

	return false;
}

bool Triangle::intersect(const Ray& ray, glm::vec3& _normal) {
	return false;
}

Raytracer::Raytracer(Surface* _surface) : m_surface(_surface)
{
	m_projectionInv = glm::perspective(75.0f, (float)_surface->getWidth() / (float)_surface->getHeight(), 0.0f, 1.0f);
	m_projectionInv = glm::inverse(m_projectionInv);
	m_viewInv = glm::mat4();

}


Raytracer::~Raytracer()
{
}

Ray Raytracer::createRay(int _pixelX, int _pixelY, float offsetX, float offsetY)
{
	Ray ray;

	//Viewing angle.
	float fov = 75.0f;
	//Aspect ratio.
	float aspect = (float)m_surface->getWidth() / (float)m_surface->getHeight();

	//Create NDC coords.
	float xNDC = (2.0f * ((float)(_pixelX + offsetX) / m_surface->getWidth()) - 1.0f) * aspect;
	float yNDC = (2.0f * ((float)(_pixelY + offsetY) / m_surface->getHeight()) - 1.0f);

	//Half the fov angle and get the tangent then multiply by the ndc to span outwards.
	float x = xNDC * glm::tan(glm::radians(fov) / 2.0f);
	float y = yNDC * glm::tan(glm::radians(fov) / 2.0f);
	
	ray.direction = glm::vec3(x, y, 1.0f) - glm::vec3(xNDC, yNDC, 0.0f);
	ray.origin = glm::vec3(xNDC, yNDC, 0.0f);

	return ray;
}

/*bool intersect(Ray _ray, Triangle triangle) {
	glm::vec3 a = triangle.m_b.m_position - triangle.m_a.m_position;
	glm::vec3 b = triangle.m_c.m_position - triangle.m_a.m_position;
	glm::vec3 cross = glm::cross(a, b);
	float area = glm::length(cross);

	float rayDirDot = glm::dot(cross, _ray.direction);
	
	if (glm::abs(rayDirDot) < 0.000001f)
		return false;

	float d = glm::dot(cross, triangle.m_a.m_position);

	float t = glm::dot(cross, _ray.origin);

	if (t < 0)
		return false;

	glm::vec3 p = _ray.origin + t * _ray.direction;

	glm::vec3 c;

	glm::vec3 edge0 = triangle.m_b.m_position - triangle.m_a.m_position;
	glm::vec3 vp0 = p - triangle.m_a.m_position;
	c = glm::cross(edge0, vp0);
	if (glm::dot(cross, c) < 0.0f) return false;

}

bool intersect(Ray _ray, Plane _plane, glm::vec3& normal) {
	return false;
}*/

void Raytracer::trace() {
	Sphere sphere;

	sphere.m_position = { 0.0f, 0.0f, 0.0f };
	sphere.m_radius = 0.15f;

	//float angle = (((float)(SDL_GetTicks() % 3000)) / 3000.0f) * (M_PI * 2);

	//m_viewInv = glm::inverse(glm::lookAt(glm::vec3(cos(angle) * 2.0f, 1.0f, sin(angle) * 2.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f)));

	static float angle = 0.0f;
	angle++;
	//m_viewInv = glm::rotate(glm::mat4(), glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
	m_viewInv = glm::lookAt(glm::vec3(glm::cos(glm::radians(angle)) * 3.0f, 0.2f, glm::sin(glm::radians(angle)) * 3.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	//m_viewInv = glm::translate(glm::mat4(), glm::vec3((glm::cos(glm::radians(angle) * 5.0f)), 0.0f, (glm::sin(glm::radians(angle)) * 5.0f)));
	//m_viewInv = glm::rotate(m_viewInv, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
	//m_viewInv = glm::translate(m_viewInv, glm::vec3(2.5f, 0.0f, 0.0f));
	//m_projectionInv = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.01f, 1000.0f); 
	//m_viewInv = glm::mat4();
	sphere.m_position = m_viewInv * glm::vec4(sphere.m_position, 1.0f);

	//cameraDir = glm::vec3(cos(angle), 0.0f, sin(angle));
	float angle2 = M_PI * 1.75f;

	for (int x = 0; x < m_surface->getWidth(); x++)
	{
		for (int y = 0; y < m_surface->getHeight(); y++)
		{
			Ray ray = createRay(x, y);

			glm::vec3 fragPos;

			if (sphere.intersect(ray, fragPos))
			{
				//float mag = glm::sqrt(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);

				/*int hitRays = 0;

				for (int ox = 0; ox < 16; ox++) {
					for (int oy = 0; oy < 16; oy++) {
						if (intersect(createRay(x, y, ((float)ox / 16.0f), ((float)oy / 16.0f)), sphere, glm::vec3()))
							hitRays++;
					}
				}

				float alpha = (float)hitRays / 256.0f;*/
				float alpha = 1.0f;

				glm::vec3 lightPos = m_viewInv * glm::vec4(cos(angle2) * 1.0f, 0.0f, sin(angle2) * 1.0f, 1.0f);

				//printf("%f, %f, %f\n", lightPos.x, lightPos.y, lightPos.z);

				glm::vec3 objectColour = glm::vec3(0.5f, 0.0f, 0.0f);

				glm::vec3 norm = glm::normalize(fragPos);
				glm::vec3 lightDir = glm::normalize(lightPos);


				float diff = glm::max(glm::dot(norm, lightDir), 0.0f);

				glm::vec3 lightCol = glm::vec3(1.0f);

				glm::vec3 ambientCol = lightCol * 0.1f;
				glm::vec3 diffuseCol = lightCol * diff;


				glm::vec3 viewDir = glm::normalize(glm::vec3(glm::cos(glm::radians(angle)) * 3.0f, 0.2f, glm::sin(glm::radians(angle)) * 3.0f) - fragPos);
				glm::vec3 reflectDir = reflect(lightDir, norm);

				float specularStrength = 0.5;
				float spec = glm::pow(glm::max(glm::dot(viewDir, reflectDir), 0.0f), 16.0f);
				glm::vec3 specular = specularStrength * spec * lightCol;

				glm::vec3 col = (ambientCol + diffuseCol + specular) * objectColour;

				m_surface->setColourAt(x, y, glm::vec4(col, alpha));
				//m_surface->setColourAt(x, y, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
			}
		}
	}
}