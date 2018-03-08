#pragma once

#include <GLM\glm.hpp>

class Surface;

struct Ray
{
	glm::vec3 origin;
	glm::vec3 direction;
};

class Raytracer
{
private:
	Surface* m_surface;
	glm::mat4 m_projectionInv;
	glm::mat4 m_viewInv;
public:
	Raytracer(Surface* _surface);
	~Raytracer();
	Ray createRay(int _pixelX, int _pixelY);
	void trace();
};


class Camera {


};



