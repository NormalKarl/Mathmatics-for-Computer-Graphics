#pragma once

#include "Scene.h"

class Rasterizer;

class RasterizerScene : public Scene
{
private:
	Rasterizer* m_renderer;
	Texture* m_img;
public:
	RasterizerScene();
	~RasterizerScene();
	virtual void draw();
};

