#include "Button.h"

#include <SDL\SDL.h>

Button::Button(Texture* icon, int x, int y, int data) : m_icon(icon), m_position({ x,y }) {
	m_size = glm::vec2(icon->getWidth() + 8, icon->getHeight() + 8);
}

Button::Button(BitmapFont* font, int x, int y, std::string text, int data) : m_font(font), m_text(text), m_position({ x, y }), m_data(data) {
	m_size = glm::vec2((float)200, (float)font->getLineHeight());
	m_icon = nullptr;
}

void Button::update()
{
	m_mouseOver = false;
	int mouseX = 0, mouseY = 0;
	SDL_GetMouseState(&mouseX, &mouseY);

	if (mouseX >= m_position.x && mouseX < m_position.x + m_size.x
		&& mouseY >= m_position.y && mouseY < m_position.y + m_size.y) {
		m_mouseOver = true;
	}
}

void Button::draw(Context& context) {
	Rasterizer::FillRect(context, m_position.x, m_position.y, m_size.x, m_size.y, m_mouseOver ? glm::uvec4(76, 175, 80, 255) : glm::uvec4(80, 183, 87, 255));

	if (m_icon != nullptr) {
		Rasterizer::DrawImage(context, m_icon, m_position.x + 4, m_position.y + 4);
	}

	if(!m_text.empty())
		m_font->drawText(context, m_text, glm::round(m_position.x + ((m_size.x - m_font->getWidth(m_text)) / 2)), m_position.y + 5, 1.0f, Filter::Point);
}