#pragma once

#include "Scene.h"
#include "Rasterizer2.h"

class Rasterizer;
class BitmapFont;
class Model;

class RasterizerScene : public Scene
{
private:
	Rasterizer* m_renderer;
	Texture* m_img;

	Context context;
	Model* model;
public:
	RasterizerScene();
	~RasterizerScene();
	virtual void draw();
};

