#pragma once

#include <vector>
#include <string>

#include "Rasterizer.h"
#include "BitmapFont.h"

class Scene;
class SceneManager;
class BitmapFont;

class TickBox {
	BitmapFont* m_font;
	std::string m_text;
	glm::vec2 m_pos;
	bool m_checked;
public:
	TickBox(BitmapFont* font, std::string text, int x, int y);
	~TickBox();
	void update();
	void draw(Context& context);
};

struct SharedAssets {
	BitmapFont* openSansFont;
	Texture* forwardIcon;
	Texture* backIcon;
	Texture* uncheckedBox;
	Texture* checkedBox;
	Texture* holdIcon;

	SharedAssets();
	~SharedAssets();
};

class SceneManager
{
private:
	int m_sceneIndex;
	std::vector<Scene*> m_scenes;
	Context m_context;
	SharedAssets* sharedAssets;

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

	inline Context& getContext() {
		return m_context;
	}

	inline SharedAssets& getSharedAssets() {
		return *sharedAssets;
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

	inline Context& getContext() {
		return SceneManager::ActiveSceneManager->getContext();
	}

	inline SharedAssets& getSharedAssets() {
		return SceneManager::ActiveSceneManager->getSharedAssets();
	}

	inline std::string getTitle() {
		return m_title;
	}

	inline void setTitle(std::string _title) {
		m_title = _title;
	}

	inline int getStartY() {
		return getSharedAssets().openSansFont->getLineHeight();
	}

};

