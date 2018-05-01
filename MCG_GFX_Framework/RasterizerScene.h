#pragma once

#include "Scene.h"

class BitmapFont;
class Model;

class RasterizerScene : public Scene
{
private:
	Context context;
	Texture* m_img;
	Model* model;
public:
	RasterizerScene();
	~RasterizerScene();
	virtual void draw();
};

