#pragma once

#include "Scene.h"
#include "Rasterizer2.h"

class Rasterizer;
class BitmapFont;

class RasterizerScene : public Scene
{
private:
	Rasterizer* m_renderer;
	Texture* m_img;

	Context context;

	Texture* m_skyboxDown;
	Texture* m_skyboxUp;
	Texture* m_skyboxFront;
	Texture* m_skyboxBack;
	Texture* m_skyboxLeft;
	Texture* m_skyboxRight;
	VertexArray m_skybox;
public:
	RasterizerScene();
	~RasterizerScene();
	virtual void draw();
};

