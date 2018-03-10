#pragma once

#include "Scene.h"

class Raytracer;

class RaytracerScene : public Scene
{
private:
	Raytracer* m_tracer;
public:
	RaytracerScene();
	~RaytracerScene();
	void draw();
};

