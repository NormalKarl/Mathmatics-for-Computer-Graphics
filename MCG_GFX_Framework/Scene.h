#pragma once

#include <vector>
#include <string>

#include "Pipeline.h"

class Scene;
class SceneManager;

class SceneManager
{
private:
	int m_sceneIndex;
	std::vector<Scene*> m_scenes;
	Surface* m_surface;
	Rasterizer m_renderer;

	std::vector<Texture*> icons;
public:
	static SceneManager* ActiveSceneManager;

	SceneManager(Surface* _surface);
	~SceneManager();

	void addScene(Scene* scene);
	void goToScene(Scene* scene);
	void goToScene(int index);

	void update();
	void draw();

	inline Surface* getSurface()
	{
		return m_surface;
	}
};

class Scene
{
private:
	std::string m_title;
public:
	Scene();
	~Scene();

	virtual void update() {}
	virtual void draw() {}

	inline SceneManager* getSceneManager()
	{
		return SceneManager::ActiveSceneManager;
	}

	inline Surface* getSurface()
	{
		return SceneManager::ActiveSceneManager->getSurface();
	}
};

