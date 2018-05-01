#pragma once

#include "Scene.h"

class PrimitivesScene : public Scene
{
private:
	Context m_renderer;
	VertexArray m_triangle;
	VertexArray m_square;
	VertexArray m_circle;
	VertexArray m_polygon;
	VertexArray m_lines;
public:
	PrimitivesScene();
	~PrimitivesScene();
	virtual void draw();
};

