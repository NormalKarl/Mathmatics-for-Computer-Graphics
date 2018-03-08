#pragma once

#include "Scene.h"

class RaytracerScene : public Scene
{
public:
	RaytracerScene();
	~RaytracerScene();
	void draw();
};

