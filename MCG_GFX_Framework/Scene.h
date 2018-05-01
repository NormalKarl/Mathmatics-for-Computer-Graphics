#pragma once

#include <vector>
#include <string>

#include "Rasterizer.h"

class Scene;
class SceneManager;
class BitmapFont;

class SceneManager
{
private:
	int m_sceneIndex;
	std::vector<Scene*> m_scenes;
	Surface* m_surface;
	Context m_renderer;

	std::vector<Texture*> icons;
	BitmapFont* m_font;
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

	inline BitmapFont* getFont() {
		return m_font;
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

	inline std::string getTitle() {
		return m_title;
	}

	inline void setTitle(std::string _title) {
		m_title = _title;
	}
};

