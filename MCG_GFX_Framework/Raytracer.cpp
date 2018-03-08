#include "Raytracer.h"
#include "Surface.h"
#include "Geometry.h"

#include "MCG_GFX_Lib.h"

#include <cstdio>

Raytracer::Raytracer(Surface* _surface) : m_surface(_surface)
{
//	glm::proj;
}


Raytracer::~Raytracer()
{
}

Ray Raytracer::createRay(int _pixelX, int _pixelY)
{
	Ray ray;

	float xNDC = ((float)_pixelX / m_surface->getWidth()) * 2.0f - 1.0f;
	float yNDC = ((float)_pixelY / m_surface->getHeight()) * 2.0f - 1.0f;

	//ray.direction = m_projectionInv * m_viewInv * glm::vec4(xNDC, yNDC, 0.0f, 1.0f);
	//ray.origin = m_projectionInv * glm::vec4(xNDC, yNDC, 0.0f, 1.0f);

	ray.direction = glm::vec3(0.0f, 0.0f, 1.0f);
	ray.origin = glm::vec3(xNDC, yNDC, 0.0f);
	return ray;
}

void intersect(Ray _ray, Sphere _sphere) {
	glm::vec3 pos = _sphere.m_position - _ray.origin;

	float d = glm::dot(pos, _ray.direction);

	//float glm::sqrt(glm::dot((d * d) - (pos * pos)));
}

void Raytracer::trace() {
	Sphere sphere;

	sphere.m_position = { -0.5f, -0.5f, 2.0f };
	sphere.m_radius = 0.3f;

	for (int x = 0; x < m_surface->getWidth(); x++)
	{
		for (int y = 0; y < m_surface->getHeight(); y++)
		{
			Ray ray = createRay(x, y);

		}
	}
}