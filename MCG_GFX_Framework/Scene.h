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
	Context m_context;

	std::vector<Texture*> icons;
	BitmapFont* m_font;

	Texture* backIcon;

	glm::vec4 backButtonRegion;
	bool mouseOnBackButton;
public:
	static SceneManager* ActiveSceneManager;

	SceneManager(Surface* _surface);
	~SceneManager();

	void addScene(Scene* scene);
	void goToScene(Scene* scene);
	void goToScene(int index);

	void update();
	void draw();

	inline std::vector<Scene*>& getScenes() {
		return m_scenes;
	}

	inline Surface* getSurface()
	{
		return m_surface;
	}

	inline Context& getContext() {
		return m_context;
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

	inline Context& getContext() {
		return SceneManager::ActiveSceneManager->getContext();
	}

	inline BitmapFont* getFont() {
		return SceneManager::ActiveSceneManager->getFont();
	}

	inline std::string getTitle() {
		return m_title;
	}

	inline void setTitle(std::string _title) {
		m_title = _title;
	}
};

