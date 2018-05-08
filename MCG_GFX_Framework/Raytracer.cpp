#include "Raytracer.h"
#include "Surface.h"
#include "Geometry.h"

#include "MCG_GFX_Lib.h"
#include <GLM\gtc\matrix_transform.hpp>
#include <GLM/glm.hpp>
#include <SDL\SDL.h>

#include <cstdio>

#include "Lighting.h"

bool Sphere::intersect(const Ray& _ray, glm::vec3& _normal) {
	glm::vec3 pos = m_position - _ray.m_origin;

	float dot = glm::dot(pos, _ray.m_direction);

	glm::vec3 distance = pos - (dot * _ray.m_direction);

	float mag = glm::sqrt(distance.x * distance.x + distance.y * distance.y + distance.z * distance.z);

	if (mag <= m_radius)
	{
		float x = glm::sqrt(m_radius * m_radius - mag * mag);
		glm::vec3 near = _ray.m_origin + ((dot - x) * _ray.m_direction);

		if (near.z > 0)
			return false;

		_normal = near - m_position;
		return true;
	}

	return false;
}

#define EPSILON 0.000001

bool Triangle::intersect(const Ray& ray, glm::vec3& _normal) {
	glm::vec3 interp;
	glm::vec3 edge1, edge2, tvec, pvec, qvec;

	float det, inv_det;

	edge1 = m_v1 - m_v0;
	edge2 = m_v2 - m_v0;

	pvec = glm::cross(ray.m_direction, edge2);
	det = glm::dot(edge1, pvec);
	inv_det = 1.0f / det;

	//Culling
	if (m_cull) {
		if (det < EPSILON)
			return false;

		tvec = ray.m_origin - m_v0;

		interp.y = glm::dot(tvec, pvec);

		if (interp.y < 0.0f || interp.y > det)
			return false;

		qvec = glm::cross(tvec, edge1);

		interp.z = glm::dot(ray.m_direction, qvec);

		if (interp.z < 0.0f || interp.y + interp.z > det)
			return false;

		interp.x = glm::dot(edge2, qvec);

		interp *= inv_det;
	} else {
		if (det > -EPSILON && det < EPSILON)
			return false;

		tvec = ray.m_origin - m_v0;

		interp.y = glm::dot(tvec, pvec) * inv_det;

		if (interp.y < 0.0f || interp.y > 1.0f)
			return false;

		qvec = glm::cross(tvec, edge1);

		interp.z = glm::dot(ray.m_direction, qvec) * inv_det;

		if (interp.z < 0.0f || interp.y + interp.z > 1.0f)
			return false;

		interp.x = glm::dot(edge2, qvec) * inv_det;
	}

	_normal = glm::cross(m_v1 - m_v0, m_v2 - m_v0);
	return true;
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
	float fov = 45.0f;
	//Aspect ratio.
	float aspect = (float)m_surface->getWidth() / (float)m_surface->getHeight();

	float halfX = (float)m_surface->getWidth() / 2;
	float halfY = (float)m_surface->getHeight() / 2;

	//Create NDC coords.
	float xNDC = ((2.0f * ((float)((float)_pixelX + offsetX) / m_surface->getWidth())) - 1.0f) * aspect;
	float yNDC = ((2.0f * ((float)((float)_pixelY + offsetY) / m_surface->getHeight())) - 1.0f);

	//Half the fov angle and get the tangent then multiply by the ndc to span outwards.
	float x = xNDC * glm::tan(glm::radians(fov) / 2.0f);
	float y = yNDC * glm::tan(glm::radians(fov) / 2.0f);
	
	//ray.direction = glm::normalize(glm::vec3(x, y, 1.0f) - glm::vec3(xNDC, yNDC, 0.0f));
	
	ray.m_direction = glm::vec3(0.0f, 0.0f, 1.0f);
	ray.m_origin = glm::vec3(xNDC, yNDC, 0.0f);

	return ray;
}

void Raytracer::trace() {
	float angle2 = M_PI * 1.75f;

	static float angle = 0.0f;
	angle += 0.2f;

	for (int x = 0; x < m_surface->getWidth(); x++)
	{
		for (int y = 0; y < m_surface->getHeight(); y++)
		{
			Ray ray = createRay(x, y);

			for (Geometry* geom : m_geom) {
				glm::vec3 fragPos;

				if (geom->intersect(ray, fragPos)) {
					glm::vec4 col = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
					Lighting::Directional(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(cos(angle), 1.0f, sin(angle)), fragPos, fragPos, col);
					m_surface->setColourAt(x, y, col);
				}
			}
		}
	}
}