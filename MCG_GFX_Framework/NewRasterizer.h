#pragma once

#include <GLM/glm.hpp>
#include "Geometry.h"

#define POINT 1
#define LINE 1
#define TRIANGLE 3

class Rasterizer {

private:
	struct {
		int points;

		void vertexShader();
		void clipping();
		void clipEdge();
		void triangulate();
		void culling();
		void raster();
		void fragShader();
		void sample();
		void blend();
	} pipeline;

public:
	void drawPoint();
	void drawLine();
	void drawTriangle();
	void drawQuad();
	void drawPolygon();
	void drawFan();
};