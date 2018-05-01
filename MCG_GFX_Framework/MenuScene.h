#pragma once

#include "Scene.h"
#include "BitmapFont.h"

class MenuButton {
private:
	BitmapFont* m_font;
	std::string m_text;
	glm::vec2 m_position;
	glm::vec2 m_size;
	bool mouseOver;
public:
	MenuButton() {}
	MenuButton(BitmapFont* font, int x, int y, std::string text);
	void update();
	void draw(Context& context);
};

class MenuScene : public Scene
{
private:
	Texture* m_backgroundTexture;
	std::vector<MenuButton> buttons;
public:
	MenuScene();
	~MenuScene();
	void update();
	void draw();
};
