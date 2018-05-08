#pragma once

#include <string>
#include <GLM\glm.hpp>
#include "Rasterizer.h"
#include "BitmapFont.h"
#include "MCG_GFX_Lib.h"

//Simple class that allows for a button containing either an icon or text.
class Button {
private:
	Texture * m_icon;
	BitmapFont* m_font;
	std::string m_text;
	glm::vec2 m_position;
	glm::vec2 m_size;
	bool m_mouseOver;
	int m_data;
public:
	Button() {}
	Button(Texture* icon, int x, int y, int data);
	Button(BitmapFont* font, int x, int y, std::string text, int data);
	void update();
	void draw(Context& context);

	inline bool isClicked() {
		return m_mouseOver & MCG::MouseClicked;
	}

	inline bool isMouseOver() {
		return m_mouseOver;
	}

	int getData() {
		return m_data;
	}
};