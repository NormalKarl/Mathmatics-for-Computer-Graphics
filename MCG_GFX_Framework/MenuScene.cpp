#include "MenuScene.h"
#include "Defs.h"
#include "Texture.h"
#include "MCG_GFX_Lib.h"

#include <SDL/SDL.h>

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
				buttons.push_back(Button(getSharedAssets().openSansFont, 200, 168 + ((i - 1) * (getSharedAssets().openSansFont->getLineHeight() + 5)), scene->getTitle(), i));
			}

			i++;
		}
	}

	for (Button& button : buttons) {
		button.update();

		if (button.isMouseOver() && MCG::MouseDown) {
			getSceneManager()->goToScene(button.getData());
		}
	}
}

void MenuScene::draw() {
	Rasterizer::DrawImage(getContext(), m_backgroundTexture, 0, 0, m_backgroundTexture->getWidth(), m_backgroundTexture->getHeight());

	for (Button& button : buttons)
		button.draw(getContext());
}
