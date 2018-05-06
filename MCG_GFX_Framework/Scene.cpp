#include "Scene.h"

#include <SDL\SDL.h>

#include "Surface.h"
#include "Defs.h"
#include "MCG_GFX_Lib.h"


SharedAssets::SharedAssets() {
	openSansFont = new BitmapFont("opensans.fnt", { "opensans_0.png" });
	backIcon = new Texture("assets/BackIcon.png");
	forwardIcon = new Texture("assets/forwardIcon.png");
	uncheckedBox = new Texture("assets/UncheckedBox.png");
	checkedBox = new Texture("assets/CheckedBox.png");
	holdIcon = new Texture("assets/HoldIcon.png");
}

SharedAssets::~SharedAssets() {
	delete openSansFont;
	delete backIcon;
	delete forwardIcon;
	delete uncheckedBox;
	delete checkedBox;
	delete holdIcon;
}


TickBox::TickBox(BitmapFont* font, std::string text, int x, int y) : m_font(font), m_text(text), m_pos({x, y}) {

}

TickBox::~TickBox() {

}

void TickBox::update() {

}

void TickBox::draw(Context& context) {
	Rasterizer::DrawImage(context, m_checked ? SceneManager::ActiveSceneManager->getSharedAssets().checkedBox : SceneManager::ActiveSceneManager->getSharedAssets().uncheckedBox
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
};
