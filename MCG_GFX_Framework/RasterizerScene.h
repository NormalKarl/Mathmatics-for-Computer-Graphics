#pragma once

#include "Scene.h"
#include "Button.h"
#include "Geometry.h"


struct ObjectEntry {
	std::string m_name;
	VertexArray m_modelData;
	glm::mat4 m_modelMatrix;
};

class RasterizerScene : public Scene
{
private:
	Context m_context;
	Texture* m_img;

	int m_lastX = 0;
	int m_lastY = 0;
	float m_cameraYaw = 0.0f;
	float m_cameraPitch = 0.0f;

	std::vector<ObjectEntry> m_entries;
	int m_entryIndex;

	Button m_prev;
	Button m_next;
	glm::vec2 m_titlePos;
public:
	RasterizerScene();
	~RasterizerScene();
	void update();
	void draw();
};

