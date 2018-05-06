#pragma once

#include "Scene.h"

class CurvesScene : public Scene
{
private:
	glm::vec2 bA, bB, bC, bD;
	glm::vec2* focusedPoint;
public:
	CurvesScene();
	~CurvesScene();
	void update();
	void draw();
};

