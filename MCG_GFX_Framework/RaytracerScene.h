#pragma once

#include "Scene.h"
#include "Raytracer.h"

class RaytracerScene : public Scene
{
private:
	Raytracer* m_tracer;
	Triangle tri;
	Sphere sphere;
public:
	RaytracerScene();
	void draw();
};

