#include "Scene.h"

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
	m_sceneIndex = -1;
	m_renderer = Rasterizer(_surface);
	m_renderer.ortho(0, m_surface->getViewport().width, m_surface->getViewport().height, 0, 0, 1);

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
	if (int i = std::find(m_scenes.begin(), m_scenes.end(), scene) != m_scenes.end())
	{
		m_sceneIndex = i - 1;
	}
	else
	{
		printf("Error: Could not find Scene in SceneManager!\n");
	}
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

}