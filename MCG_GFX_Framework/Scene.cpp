#include "Scene.h"

#include <SDL\SDL.h>

#include "Surface.h"
#include "Defs.h"
#include "MCG_GFX_Lib.h"


SharedAssets::SharedAssets() {
	openSansFont = new BitmapFont("resources/textures/opensans.fnt", { "resources/textures/opensans_0.png" });
	backIcon = new Texture("resources/textures/BackIcon.png");
	forwardIcon = new Texture("resources/textures/forwardIcon.png");
	uncheckedBox = new Texture("resources/textures/UncheckedBox.png");
	checkedBox = new Texture("resources/textures/CheckedBox.png");
	holdIcon = new Texture("resources/textures/HoldIcon.png");
	staticIcon = new Texture("resources/textures/StaticIcon.png");
	axisBackground = new Texture("resources/textures/AxisBackground.png");
	menuBackground = new Texture("resources/textures/MainMenu.png", Filter::Point);
}

SharedAssets::~SharedAssets() {
	delete openSansFont;
	delete backIcon;
	delete forwardIcon;
	delete uncheckedBox;
	delete checkedBox;
	delete holdIcon;
	delete staticIcon;
	delete axisBackground;
	delete menuBackground;
}

TickBox::TickBox(BitmapFont* font, std::string text, int x, int y) : m_font(font), m_text(text), m_pos({x, y}), m_checked(false) {

}

TickBox::~TickBox() {

}

void TickBox::update() {
	Texture* checkBox = SceneManager::ActiveSceneManager->getSharedAssets().checkedBox;

	int mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);

	if (MCG::MouseClicked)
	{
		if (mouseX >= m_pos.x && mouseY >= m_pos.y
			&& mouseX < m_pos.x + checkBox->getWidth()
			&& mouseY < m_pos.y + checkBox->getHeight())
		{
			m_checked = !m_checked;
		}
	}
}

void TickBox::draw(Context& context) {
	Texture* checkBox = SceneManager::ActiveSceneManager->getSharedAssets().checkedBox;
	Texture* uncheckedBox = SceneManager::ActiveSceneManager->getSharedAssets().uncheckedBox;
	Rasterizer::DrawImage(context, m_checked ? checkBox : uncheckedBox, m_pos.x, m_pos.y, 16, 16);
	m_font->drawText(context, m_text, m_pos.x + 20, m_pos.y + 2, 0.65f, Filter::Bilinear, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
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

SceneManager::SceneManager(Surface* _surface)
{
	sharedAssets = new SharedAssets();
	m_context = Context(_surface);
	m_context.ortho(0, m_context.getWidth(), m_context.getHeight(), 0, 0, 1);
	m_sceneIndex = -1;

	float offsetY = (sharedAssets->openSansFont->getLineHeight() - 24.0f) / 2.0f;
	float offsetX = offsetY * 2;
	backButtonRegion = {0,0, offsetX * 2 + 29.0f + ((sharedAssets->openSansFont->getWidth("Back")) * 0.75f), sharedAssets->openSansFont->getLineHeight() };

	ActiveSceneManager = this;


	tickboxBlur = new TickBox(sharedAssets->openSansFont, "Gaussian Blur", 10, _surface->getHeight() - 50);

}

SceneManager::~SceneManager()
{
	delete sharedAssets;
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


	tickboxBlur->update();

	m_context.m_surface->setBlurEnabled(tickboxBlur->isChecked());

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

	//Reset the most commonly used matrices inside the context.
	m_context.m_view = glm::mat4();
	m_context.m_world = glm::mat4();
	m_context.m_model = glm::mat4();

	Rasterizer::FillRect(m_context, 0.0f, 0.0f, m_context.getWidth(), sharedAssets->openSansFont->getLineHeight(), glm::uvec4(53, 83, 125, 255));

	sharedAssets->openSansFont->drawText(m_context, m_scenes[m_sceneIndex]->getTitle(), (m_context.getWidth() - sharedAssets->openSansFont->getWidth(m_scenes[m_sceneIndex]->getTitle())) / 2, 6, 1.0f);

	if (m_sceneIndex != 0) {
		float offsetY = (sharedAssets->openSansFont->getLineHeight() - 24.0f) / 2.0f;
		float offsetX = offsetY * 2;
		Rasterizer::FillRect(m_context, 0.0f, 0.0f, backButtonRegion.p, backButtonRegion.q, mouseOnBackButton ? glm::uvec4(30, 48, 72, 255) : glm::uvec4(68, 107, 160, 255));
		Rasterizer::DrawImage(m_context, sharedAssets->backIcon, offsetX, offsetY, 24.0f, 24.0f);
		sharedAssets->openSansFont->drawText(m_context, "Back", round(offsetX + 29.0f), 9, 0.75f, Filter::Bilinear);
	}

	tickboxBlur->draw(m_context);
};
