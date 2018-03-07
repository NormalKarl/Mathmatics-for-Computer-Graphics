#include "Raytracer.h"
#include "Surface.h"

Raytracer::Raytracer(Surface* _surface) : m_surface(_surface)
{
}


Raytracer::~Raytracer()
{
}

Ray Raytracer::createRay(int _pixelX, int _pixelY)
{
	Ray ray;

	float xNDC = ((float)_pixelX / m_surface->getWidth()) * 2.0f - 1.0f;
	float yNDC = ((float)_pixelY / m_surface->getHeight()) * 2.0f - 1.0f;

	ray.direction = m_projectionInv * m_viewInv * glm::vec4(xNDC, yNDC, 0.0f, 1.0f);
	ray.origin = m_projectionInv * glm::vec4(xNDC, yNDC, 0.0f, 1.0f);
}
