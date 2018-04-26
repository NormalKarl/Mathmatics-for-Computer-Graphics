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
	if (m_sceneIndex != -1)
		m_scenes[m_sceneIndex]->update();
}

void SceneManager::draw()
{
	if (m_sceneIndex != -1)
		m_scenes[m_sceneIndex]->draw();

	/*for (int i = 0; i < 5; i++)
	{
		Rectangle rectangle(glm::round((m_surface->getViewport().width / 2) + (i * 35 + (i * 5)) - (2.5f * 35)), m_surface->getViewport().height - 45, 35, 35);
		rectangle.setTexture(icons[0]);
		rectangle.setColour({ 1, 1, 1, 0.75f });
		rectangle.draw(&m_renderer);
	}*/

	m_renderer.bindTexture(NULL, 0);

	glm::vec3 col = glm::vec3(53.0f, 83.0f, 125.0f) / glm::vec3(255.0f, 255.0f, 255.0f);

	Vertex vertices[] = { Vertex(0, 0, 0.0f, col.r, col.g, col.b, 1.0f),
		Vertex(m_surface->getWidth(), 0, 0.0f, col.r, col.g, col.b, 1.0f) ,
		Vertex(m_surface->getWidth(), m_font->getLineHeight(), 0.0f, col.r, col.g, col.b, 1.0f) ,
		Vertex(0, m_font->getLineHeight(), 0.0f, col.r, col.g, col.b, 1.0f) };
	m_renderer.drawQuad(vertices[0], vertices[1], vertices[2], vertices[3]);
	m_font->drawText(&m_renderer, m_scenes[m_sceneIndex]->getTitle(), (m_surface->getWidth() - m_font->getWidth(m_scenes[m_sceneIndex]->getTitle())) / 2, 5);

	/*for (int i = 0; i < m_scenes.size(); i++) {
		int h = m_font->getLineHeight();
		int w = m_font->getWidth(m_scenes[i]->getTitle());

		Vertex vertices[] = { Vertex(5.0f, 5.0f + ((5 + h) * i), 0.0f, 0.0f, 0.0f, 0.0f, 0.5f),
			Vertex(5.0f + w, 5.0f + ((5 + h) * i), 0.0f, 0.0f, 0.0f, 0.0f, 0.5f) ,
			Vertex(5.0f + w, 5.0f + ((5 + h) * i) + h, 0.0f, 0.0f, 0.0f, 0.0f, 0.5f) ,
			Vertex(5.0f, 5.0f + ((5 + h) * i) + h, 0.0f, 0.0f, 0.0f, 0.0f, 0.5f) };
		m_renderer.drawQuad(vertices[0], vertices[1], vertices[2], vertices[3]);

		m_font->drawText(&m_renderer, m_scenes[i]->getTitle(), 5, 5 + ((5 + h) * i));


	}*/


}