#include "Scene.h"

#include "BitmapFont.h"

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

	m_font = new BitmapFont("opensans.fnt", { "opensans_0.png" });
	m_sceneIndex = -1;
	m_renderer = Rasterizer(_surface);
	m_renderer.ortho(0, m_surface->getViewport().width, m_surface->getViewport().height, 0, 0, 1);
	m_renderer.setCulling(Rasterizer::Culling::Backface, Rasterizer::WindingOrder::Clockwise);

	icons.push_back(new Texture("icon1.png"));

	ActiveSceneManager = this;
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
	if(m_sceneIndex != -1)
		m_scenes[m_sceneIndex]->update();
}

void SceneManager::draw()
{
	if (m_sceneIndex != -1)
		m_scenes[m_sceneIndex]->draw();

	for (int i = 0; i < 5; i++)
	{
		Rectangle rectangle(glm::round((m_surface->getViewport().width / 2) + (i * 35 + (i * 5)) - (2.5f * 35)), m_surface->getViewport().height - 45, 35, 35);
		//Rectangle rectangle((m_surface->getViewport().width / 2) + (i * 35 + (i * 5)) - (2.5f * 35), m_surface->getViewport().height - 45, 35, 35);
		rectangle.setTexture(icons[0]);
		rectangle.setColour({ 1, 1, 1, 0.75f });
		rectangle.draw(&m_renderer);
	}

	m_font->drawText(&m_renderer, m_scenes[m_sceneIndex]->getTitle(), 5, 5);

}