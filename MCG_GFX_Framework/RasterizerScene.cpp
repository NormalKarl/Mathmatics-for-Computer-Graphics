#include "RasterizerScene.h"
#include "Pipeline.h"
#include "MCG_GFX_Lib.h"
#include <SDL\SDL.h>

#define USE_MATH_DEFINES

#include <cmath>

std::vector<Vertex> vertices = {
	{ 0, 0, 0, 0, 1 },
	{ 0, 1, 0, 0, 0 },
	{ 1, 1, 0, 1, 0 },

{ 1, 1, 0, 1, 0 },
	{ 0, 0, 0, 0, 1 },
{ 0, 0, 0, 0, 1 },
};

RasterizerScene::RasterizerScene()
{
	m_renderer = new Rasterizer(getSurface());
	m_img = new Texture("tex2.png");
}


RasterizerScene::~RasterizerScene()
{
}

int lastMouseX = 0, lastMouseY = 0;

float angle = 0.0f;
float angleY = 0.0f;
float dist = 2.5f;

bool wireframe;

void RasterizerScene::draw()
{

	if (MCG::MouseDown)
	{
		int tempX = 0, tempY = 0;
		SDL_GetMouseState(&tempX, &tempY);

		int diffX = tempX - lastMouseX, diffY = tempY - lastMouseY;

		angle += diffX / 180.0f * M_PI;
		angleY += diffY / 180.0f * M_PI;

		lastMouseX = tempX;
		lastMouseY = tempY;
		wireframe = true;
	}
	else
	{
		wireframe = false;
		SDL_GetMouseState(&lastMouseX, &lastMouseY);
	}

	float aspect = (float)getSurface()->getViewport().width / (float)getSurface()->getViewport().height;

	//dist = (1.0f - glm::abs(glm::cos(angleY))) * 5.0f;

	//https://gamedev.stackexchange.com/questions/20758/how-can-i-orbit-a-camera-about-its-target-point
	if (dist < 0.00001f)
	{
		dist = 0.00001f;
	}

	Uint32 ticks = 0;

	m_renderer->perspective(glm::radians(75.0f), aspect, 0.1f, 100.0f);
	m_renderer->lookAt(glm::cos(angle) * dist, glm::cos(angleY) * 5.0f, glm::sin(angle) * dist, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	
	Vertex a = { 0.5, 0.5f, -0.5f, 1.0f, 1.0f };
	Vertex b = { -0.5, 0.5f, -0.5f, 0.0f, 1.0f };
	Vertex c = { -0.5, -0.5, -0.5f, 0.0f, 0.0f };

	Vertex d = { -0.5f, -0.5f, -0.5f, 0.0f, 0.0f };
	Vertex e = { 0.5f, -0.5f, -0.5f, 1.0f, 0.0f };
	Vertex f = { 0.5f, 0.5f, -0.5f, 1.0f, 1.0f };

	m_renderer->bindTexture(m_img);

	m_renderer->setModel(glm::rotate(glm::mat4(), glm::radians(ticks / 50.0f), { 0.0f, 1.0f, 0.0f }));
	m_renderer->drawTriangle(a, b, c);
	m_renderer->drawTriangle(d, e, f);
	m_renderer->setModel(glm::rotate(glm::mat4(), glm::radians((ticks / 50.0f) + 90.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
	m_renderer->drawTriangle(a, b, c);
	m_renderer->drawTriangle(d, e, f);
	m_renderer->setModel(glm::rotate(glm::mat4(), glm::radians((ticks / 50.0f) + 180.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
	m_renderer->drawTriangle(a, b, c);
	m_renderer->drawTriangle(d, e, f);
	m_renderer->setModel(glm::rotate(glm::mat4(), glm::radians((ticks / 50.0f) + 270.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
	m_renderer->drawTriangle(a, b, c);
	m_renderer->drawTriangle(d, e, f);

	Vertex h = { 0.5, 0.5f, 0.5f, 1.0f, 1.0f };
	Vertex i = { -0.5, 0.5f, 0.5f, 0.0f, 1.0f };
	Vertex j = { -0.5, 0.5f, -0.5, 0.0f, 0.0f };

	Vertex k = { -0.5f, 0.5f, -0.5f, 0.0f, 0.0f };
	Vertex l = { 0.5f, 0.5f, -0.5f, 1.0f, 0.0f };
	Vertex m = { 0.5f, 0.5f, 0.5f, 1.0f, 1.0f };

	m_renderer->setModel(glm::rotate(glm::mat4(), glm::radians(ticks / 50.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
	m_renderer->drawTriangle(h, i, j);
	m_renderer->drawTriangle(k, l, m);

	m_renderer->setModel(glm::rotate(glm::mat4(), glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
	m_renderer->drawTriangle(h, i, j);
	m_renderer->drawTriangle(k, l, m);
}