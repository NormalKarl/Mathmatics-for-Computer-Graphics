#pragma once

#include "Scene.h"
#include "Button.h"

class MenuScene : public Scene
{
private:
	std::vector<Button> buttons;
public:
	MenuScene();
	~MenuScene();
	void update();
	void draw();
};
