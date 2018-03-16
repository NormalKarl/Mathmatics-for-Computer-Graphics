#include "Raytracer.h"
#include "Surface.h"
#include "Geometry.h"

#include "MCG_GFX_Lib.h"
#include <GLM\gtc\matrix_transform.hpp>
#include <GLM/glm.hpp>

#include <cstdio>

Raytracer::Raytracer(Surface* _surface) : m_surface(_surface)
{
	m_projectionInv = glm::perspective(75.0f, (float)_surface->getWidth() / (float)_surface->getHeight(), 0.0f, 1.0f);
	m_viewInv = glm::mat4();
}


Raytracer::~Raytracer()
{
}

Ray Raytracer::createRay(int _pixelX, int _pixelY)
{
	Ray ray;

	float fov = 75.0f;
	float aspect = (float)m_surface->getWidth() / (float)m_surface->getHeight();
	float xNDC = (2.0f * ((float)(_pixelX + 0.5f) / m_surface->getWidth()) - 1.0f) * aspect;
	float yNDC = (2.0f * ((float)(_pixelY + 0.5f) / m_surface->getHeight()) - 1.0f);
	float x = xNDC * glm::tan(glm::radians(fov) / 2);
	float y = yNDC * glm::tan(glm::radians(fov) / 2);
	
	//float xNDC = ((float)(_pixelX + 0.5f) / m_surface->getWidth()) * 2.0f - 1.0f;
	//float yNDC = ((float)(_pixelY + 0.5f) / m_surface->getHeight()) * 2.0f - 1.0f;

	//xNDC *= aspect;

	//ray.direction = m_projectionInv * m_viewInv * glm::vec4(xNDC, yNDC, 0.0f, 1.0f);
	//ray.origin = m_projectionInv * glm::vec4(xNDC, yNDC, 0.0f, 1.0f);

	//float Px = (2 * (((float)_pixelX + 0.5) / (float)m_surface->getWidth()) - 1) * glm::tan(fov / 2 * glm::pi<float>() / 180) * aspect;
	//float Py = (1 - 2 * (((float)_pixelY + 0.5) / (float)m_surface->getHeight()) * glm::tan(fov / 2 * glm::pi<float>() / 180));



	ray.direction = glm::vec3(x, y, 1.0f) - glm::vec3(xNDC, yNDC, 0.0f);
	ray.origin = glm::vec3(xNDC, yNDC, 0.0f);


	return ray;
}

bool intersect(Ray _ray, Sphere _sphere , glm::vec3& normal) {
	glm::vec3 pos = _sphere.m_position - _ray.origin;

	float dot = glm::dot(pos, _ray.direction);

	glm::vec3 distance = _sphere.m_position - _ray.origin - (dot * _ray.direction);

	float mag = glm::sqrt(distance.x * distance.x + distance.y * distance.y);

	if (mag <= _sphere.m_radius)
	{
		float x = glm::sqrt(_sphere.m_radius * _sphere.m_radius - mag * mag);
		glm::vec3 near = _ray.origin + ((dot - x) * _ray.direction);

		normal = glm::normalize(near - _sphere.m_position);
		//printf("%f\n", near.z);
		return true;
	}

	return false;
}

bool intersect(Ray _ray, Plane _plane, glm::vec3& normal) {
	return false;
}

void Raytracer::trace() {
	Sphere sphere;

	sphere.m_position = { -0.1f, -0.1f, 2.0f };
	sphere.m_radius = 0.15f;

	for (int x = 0; x < m_surface->getWidth(); x++)
	{
		for (int y = 0; y < m_surface->getHeight(); y++)
		{
			Ray ray = createRay(x, y);

			glm::vec3 normal;

			

			if (intersect(ray, sphere, normal))
			{
				//float mag = glm::sqrt(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);

				float diff = glm::clamp(glm::dot(normal, {0.0f, 1.0f, 0.0f}), 0.0f, 0.7f);
				glm::vec4 col = diff * glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

				m_surface->setColourAt(x, y, ((1.0f - diff) * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)) + col);
			}
		}
	}
}