#include "Scene.h"

#include <SDL\SDL.h>

#include "BitmapFont.h"
#include "Surface.h"
#include "Defs.h"
#include "MCG_GFX_Lib.h"


TickBox::TickBox(BitmapFont* font, std::string text, int x, int y) : m_font(font), m_text(text), m_pos({x, y}) {

}

TickBox::~TickBox() {

}

void TickBox::update() {

}

void TickBox::draw(Context& context) {
	Rasterizer::DrawImage(context, m_checked ? SceneManager::ActiveSceneManager->getCheckedBoxTexture() : SceneManager::ActiveSceneManager->getUncheckedBoxTexture()
		, m_pos.x, m_pos.y, 16, 16);
	//Rasterizer::DrawImage();
}

//Scene Manager

SceneManager* SceneManager::ActiveSceneManager = NULL;

Scene::Scene()
{
	m_title = "Untitled Scene";
}

Scene::~Scene()
{
}

SceneManager::SceneManager(Surface* _surface) : m_surface(_surface)
{

	m_font = OPEN_SANS_FONT_INIT;
	m_sceneIndex = -1;
	m_context.m_surface = _surface;
	m_context.ortho(0, m_surface->getViewport().width, m_surface->getViewport().height, 0, 0, 1);
	//m_renderer.setCulling(Rasterizer::Culling::Backface, Rasterizer::WindingOrder::Clockwise);

	icons.push_back(new Texture("icon1.png"));
	backIcon = BACK_ICON_INIT;

	float offsetY = (m_font->getLineHeight() - 24.0f) / 2.0f;
	float offsetX = offsetY * 2;
	backButtonRegion = {0,0, offsetX * 2 + 29.0f + ((m_font->getWidth("Back")) * 0.75f), m_font->getLineHeight() };

	ActiveSceneManager = this;

	uncheckedBox = new Texture("assets/UncheckedBox.png");
	checkedBox = new Texture("assets/CheckedBox.png");
}

SceneManager::~SceneManager()
{

}

void SceneManager::addScene(Scene* scene)
{
	m_scenes.push_back(scene);
}

void SceneManager::goToScene(Scene* scene)
{
	bool foundEntry = false;
	for (int i = 0; i < m_scenes.size(); i++)
	{
		if (m_scenes[i] == scene)
		{
			m_sceneIndex = i;
			foundEntry = true;
		}
	}

	if (!foundEntry)
		printf("Error: Couldn't find scene with Scene pointer.");
}

void SceneManager::goToScene(int index)
{
	if (index >= 0 && index < m_scenes.size())
	{
		m_sceneIndex = index;
	}
	else
	{
		printf("Error: Index is out of range!");
	}
}

void SceneManager::update()
{
	if (m_sceneIndex != -1)
		m_scenes[m_sceneIndex]->update();

	int mouseX = 0, mouseY = 0;
	mouseOnBackButton = false;
	SDL_GetMouseState(&mouseX, &mouseY);

	if (m_sceneIndex != 0) {
		if (mouseX >= backButtonRegion.x && mouseX < backButtonRegion.x + backButtonRegion.z
			&& mouseY >= backButtonRegion.y && mouseY < backButtonRegion.y + backButtonRegion.w) {
			mouseOnBackButton = true;

			if (MCG::MouseDown) {
				goToScene(0);
			}
		}
	}
}

void SceneManager::draw()
{
	if (m_sceneIndex != -1)
		m_scenes[m_sceneIndex]->draw();


	Rasterizer::FillRect(m_context, 0.0f, 0.0f, m_surface->getWidth(), m_font->getLineHeight(), glm::uvec4(53, 83, 125, 255));

	m_font->drawText(m_context, m_scenes[m_sceneIndex]->getTitle(), (m_surface->getWidth() - m_font->getWidth(m_scenes[m_sceneIndex]->getTitle())) / 2, 6, 1.0f);

	if (m_sceneIndex != 0) {
		float offsetY = (m_font->getLineHeight() - 24.0f) / 2.0f;
		float offsetX = offsetY * 2;
		Rasterizer::FillRect(m_context, 0.0f, 0.0f, backButtonRegion.p, backButtonRegion.q, mouseOnBackButton ? glm::uvec4(30, 48, 72, 255) : glm::uvec4(68, 107, 160, 255));
		Rasterizer::DrawImage(m_context, backIcon, offsetX, offsetY, 24.0f, 24.0f);
		m_font->drawText(m_context, "Back", round(offsetX + 29.0f), 9, 0.75f, Filter::Bilinear);
	}
};
