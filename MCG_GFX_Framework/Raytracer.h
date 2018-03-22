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

	glm::vec3 cameraDir;
public:
	Raytracer(Surface* _surface);
	~Raytracer();
	Ray createRay(int _pixelX, int _pixelY, float offsetX = 0.5f, float offsetY = 0.5f);
	void trace();
};


class Camera {


};



