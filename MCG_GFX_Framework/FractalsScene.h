#pragma once

#include "Scene.h"

class FractalsScene : public Scene
{
private:
	Context* context;
	Texture* mandlebrot;
	Texture* seripinski;
public:
	FractalsScene();
	~FractalsScene();
	void draw();
};

