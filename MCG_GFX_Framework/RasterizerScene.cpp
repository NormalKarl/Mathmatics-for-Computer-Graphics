#include "RasterizerScene.h"
#include "Pipeline.h"
#include "MCG_GFX_Lib.h"
#include <SDL\SDL.h>

#define USE_MATH_DEFINES

#include <cmath>

std::vector<Vertex> vertices;

RasterizerScene::RasterizerScene()
{
	m_renderer = new Rasterizer(getSurface());
	m_renderer->setCulling(Rasterizer::Culling::None, Rasterizer::WindingOrder::Clockwise);
	m_img = new Texture("tex2.png");

	Texture heightMap = Texture("heightmap.png");
	heightMap.filter = Filter::Point;

	for (int x = 1; x < heightMap.width - 1; x++)
	{
		for (int y = 1; y < heightMap.height - 1; y++)
		{
			glm::vec4 pixel = heightMap.getPixelAt(x, y);
			glm::vec4 pixelRight = heightMap.getPixelAt(x + 1, y);
			glm::vec4 pixelDown = heightMap.getPixelAt(x, y + 1);
			glm::vec4 pixelRightDown = heightMap.getPixelAt(x + 1, y + 1);

			Vertex a = { (float)x, pixel.g * 10.0f, (float)y, pixel.g, pixel.g, pixel.g, 1.0f };
			Vertex b = { (float)x + 1, pixelRight.g * 10.0f, (float)y, pixel.g, pixel.g, pixel.g, 1.0f };
			Vertex c = { (float)x, pixelDown.g * 10.0f, (float)y + 1, pixel.g, pixel.g, pixel.g, 1.0f };


			Vertex d = { (float)x + 1, pixelRight.g * 10.0f, (float)y, pixel.g, pixel.g, pixel.g, 1.0f };
			Vertex e = { (float)x + 1, pixelRightDown.g * 10.0f, (float)y + 1, pixel.g, pixel.g, pixel.g, 1.0f };
			Vertex f = { (float)x, pixelDown.g * 10.0f, (float)y + 1, pixel.g, pixel.g, pixel.g, 1.0f };

			vertices.push_back(a);
			vertices.push_back(b);
			vertices.push_back(c);
			vertices.push_back(d);
			vertices.push_back(e);
			vertices.push_back(f);
		}
	}
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
	m_renderer->setModel(glm::scale(glm::mat4(), glm::vec3(0.1f, 0.1f, 0.1f)));
	m_renderer->bindTexture(NULL);

	for (int i = 0; i < vertices.size() / 3; i++)
	{
		m_renderer->drawTriangle(vertices[i * 3 + 0], vertices[i * 3 + 1], vertices[i * 3 + 2]);
	}
	
	/*Vertex a = { 0.5, 0.5f, -0.5f, 1.0f, 1.0f };
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
	m_renderer->drawTriangle(k, l, m);*/
}