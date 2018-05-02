#pragma once

#include <GLM\glm.hpp>

#include <vector>

class Surface;

struct Ray
{
	glm::vec3 origin;
	glm::vec3 direction;
};

struct Geometry {
	virtual bool intersect(const Ray& ray, glm::vec3& _normal) = 0;
};

struct Sphere : public Geometry {
public:
	glm::vec3 m_position;
	float m_radius;
	bool intersect(const Ray& ray, glm::vec3& _normal);
};

class Triangle : public Geometry {
public:
	glm::vec3 v0, v1, v2;
	bool intersect(const Ray& ray, glm::vec3& _normal);
};

struct World
{
	std::vector<Geometry> m_objects;
};

class Raytracer
{
private:
	Surface* m_surface;
	glm::mat4 m_projectionInv;
	glm::mat4 m_viewInv;

	glm::vec3 cameraDir;
public:
	Raytracer(Surface* _surface);
	~Raytracer();
	Ray createRay(int _pixelX, int _pixelY, float offsetX = 0.5f, float offsetY = 0.5f);
	void trace();
};
