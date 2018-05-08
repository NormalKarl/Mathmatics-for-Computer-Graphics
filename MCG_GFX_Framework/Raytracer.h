#ifndef _RAYTRACER_H_
#define _RAYTRACER_H_

#include <GLM\glm.hpp>

#include <vector>

class Surface;

/*
Ray is used for intersection tests containing an origin and a direciton.
*/
struct Ray
{
	glm::vec3 m_origin;
	glm::vec3 m_direction;
};

/*
Geometry is a interface that will allow the raytracer to test intersections
with different shapes.
*/
struct Geometry
{
	virtual bool intersect(const Ray& _ray, glm::vec3& _normal) = 0;
};

struct Sphere : public Geometry
{
	glm::vec3 m_position;
	float m_radius;

	bool intersect(const Ray& _ray, glm::vec3& _normal);
};

struct Triangle : public Geometry
{
	bool m_cull;
	glm::vec3 m_v0;
	glm::vec3 m_v1;
	glm::vec3 m_v2;

	bool intersect(const Ray& _ray, glm::vec3& _normal);
};

/*
Raytracer holds the world geometry defined above and will perform
a raytracing algorihm to draw the pixels to the screen.
*/
class Raytracer
{
private:
	Surface* m_surface;
	glm::mat4 m_projectionInv;
	glm::mat4 m_viewInv;
	std::vector<Geometry*> m_geom;
public:
	Raytracer(Surface* _surface);
	~Raytracer();
	Ray createRay(int _pixelX, int _pixelY, float _offsetX = 0.5f, float _offsetY = 0.5f);
	void trace();
	inline void addGeom(Geometry* _geom) { m_geom.push_back(_geom); }
};

#endif