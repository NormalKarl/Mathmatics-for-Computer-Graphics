#pragma once

#include <GLM/glm.hpp>
#include "Geometry.h"

class Rasterizer {
public:
	void drawPoint();
	void drawLine();
	void drawTriangle();
};