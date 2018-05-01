#include "MenuScene.h"
#include "Defs.h"
#include "Texture.h"

#include <SDL/SDL.h>

MenuButton::MenuButton(BitmapFont* font, int x, int y, std::string text) : m_font(font), m_text(text), m_position({ x, y }) {
	m_size = glm::vec2((float)200, (float)font->getLineHeight());
}

void MenuButton::update() {
	mouseOver = false;
	int mouseX = 0, mouseY = 0;
	SDL_GetMouseState(&mouseX, &mouseY);

	if (mouseX >= m_position.x && mouseX < m_position.x + m_size.x
		&& mouseY >= m_position.y && mouseY < m_position.y + m_size.y) {
		mouseOver = true;
	}


}

void MenuButton::draw(Context& context) {
	Rasterizer::FillRect(context, m_position.x, m_position.y, m_size.x, m_size.y, glm::uvec4(0,0,0,255));
	m_font->drawText(context, m_text, glm::round(m_position.x + ((m_size.x - m_font->getWidth(m_text)) / 2)), m_position.y + 5, 1.0f, Filter::Point);
}

MenuScene::MenuScene()
{
	setTitle("Menu");
	m_backgroundTexture = MAIN_MENU_BG_INIT;
}

MenuScene::~MenuScene()
{
	delete m_backgroundTexture;
}

void MenuScene::update() {
	if (buttons.empty()) {
		int i = 0;
		for (Scene* scene : getSceneManager()->getScenes()) {
			if (scene != this) {
				buttons.push_back(MenuButton(getFont(), 350, 250 + ((i - 1) * (getFont()->getLineHeight() + 5)), scene->getTitle()));
			}

			i++;
		}
	}

	for (MenuButton& button : buttons)
		button.update();
}

void MenuScene::draw() {
	Rasterizer::DrawImage(getContext(), m_backgroundTexture, 0, 0, m_backgroundTexture->getWidth(), m_backgroundTexture->getHeight());

	for (MenuButton& button : buttons)
		button.draw(getContext());
}
