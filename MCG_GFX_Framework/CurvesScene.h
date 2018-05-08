#pragma once

#include "Scene.h"
#include "Button.h"

class CurvesScene : public Scene
{
private:
	glm::vec2 pointA, pointB, pointC, pointD;
	glm::vec2* focusedPoint;
	Button m_prev;
	Button m_next;
	glm::vec2 m_titlePos;
	int m_exampleNo;
public:
	CurvesScene();
	~CurvesScene() {}
	void update();
	void draw();
};

