#pragma once

#include "Scene.h"

class PrimitivesScene : public Scene
{
private:
	Rasterizer m_renderer;
	VertexArray m_polygonArray;
public:
	PrimitivesScene();
	~PrimitivesScene();
	virtual void draw();
};

