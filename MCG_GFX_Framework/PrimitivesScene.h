#pragma once

#include "Scene.h"
#include "Geometry.h"

class PrimitivesScene : public Scene
{
private:
	VertexArray m_triangle;
	VertexArray m_square;
	VertexArray m_circle;
	VertexArray m_polygon;
	VertexArray m_lines;
public:
	PrimitivesScene();
	~PrimitivesScene() { }
	virtual void draw();
};
