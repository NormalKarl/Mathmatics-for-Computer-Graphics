#pragma once

#include <GLM/glm.hpp>
#include "Geometry.h"

class Rasterizer {

private:

	void vertexShader();
	void clipping();
	void clipEdge();
	void triangulate();
	void culling();
	void raster();
	void fragShader();
	void sample();
	void blend();

public:
	void drawPoint();
	void drawLine();
	void drawTriangle();
	void drawQuad();
	void drawPolygon();
	void drawFan();
};