#pragma once

#include "Scene.h"
#include "Button.h"

struct FractalEntry;

class FractalsScene : public Scene
{
private:
	Context& m_context;
	std::vector<FractalEntry*> m_fractals;
	int m_fractalIndex;
	Button m_prev;
	Button m_next;
	glm::vec2 m_fractalTitlePos;
public:
	FractalsScene();
	~FractalsScene();
	void update();
	void draw();
};

