#include "RasterizerScene.h"
#include "Rasterizer.h"
#include "MCG_GFX_Lib.h"
#include <SDL\SDL.h>

#define USE_MATH_DEFINES

#include "BitmapFont.h"
#include "Geometry.h"
#include "Surface.h"
#include <cmath>

VertexArray va;

RasterizerScene::RasterizerScene()
{
	m_context.m_surface = getContext().m_surface;
	float aspect = (float)getContext().getWidth() / (float)m_context.getHeight();
	m_context.perspective(glm::radians(75.0f), aspect, 1.0f, 2048.0f);
	setTitle("Rasterizer");

	Texture heightMap = Texture("resources/textures/heightmap.png");
	heightMap.setFilter(Filter::Point);

	float gradient = 20.0f;

	for (int x = 0; x < heightMap.getWidth() - 1; x++)
	{
		for (int y = 0; y < heightMap.getHeight() - 1; y++)
		{
			glm::vec4 pixel = heightMap.getPixelAt(x, y);
			glm::vec4 pixelRight = heightMap.getPixelAt(x + 1, y);
			glm::vec4 pixelDown = heightMap.getPixelAt(x, y + 1);
			glm::vec4 pixelRightDown = heightMap.getPixelAt(x + 1, y + 1);

			Vertex a = { (float)x, pixel.g * gradient, (float)y };
			Vertex b = { (float)x + 1, pixelRight.g * gradient, (float)y };
			Vertex c = { (float)x, pixelDown.g * gradient, (float)y + 1 };

			Vertex d = { (float)x + 1, pixelRight.g * gradient, (float)y };
			Vertex e = { (float)x + 1, pixelRightDown.g * gradient, (float)y + 1 };
			Vertex f = { (float)x, pixelDown.g * gradient, (float)y + 1 };

			va.appendVertex(a);
			va.appendVertex(b);
			va.appendVertex(c);
			va.appendVertex(d);
			va.appendVertex(e);
			va.appendVertex(f);
		}
	}

	m_entries.push_back({ "Crate", Model::CreateBox(new Texture("resources/textures/Container.png", Filter::Bilinear)), glm::translate(glm::mat4(), glm::vec3(0.0f, 1.0f, 0.0f)) });
	m_entries.push_back({ "Height Map", va, glm::translate(glm::scale(glm::mat4(), glm::vec3(0.1f, 0.1f, 0.1f)), glm::vec3(-heightMap.getWidth() / 2, 0.0f, -heightMap.getHeight() / 2)) });
	m_entries.push_back({ "Stanford Bunny", Model::LoadOBJ("resources/models/bunny.wobj"), glm::scale(glm::translate(glm::mat4(), glm::vec3(0.0f, -0.5f, 0.0f)), glm::vec3(12.5f)) });
	m_entries.push_back({ "Stanford Teapot", Model::LoadOBJ("resources/models/teapot.wobj"), glm::scale(glm::translate(glm::mat4(), glm::vec3(0.0f, 0.5f, 0.0f)), glm::vec3(0.4f)) });
	m_entryIndex = 0;

	//This is just some arbritary placement for the buttons.
	float h = m_context.getHeight() - getSharedAssets().openSansFont->getLineHeight() * 2;

	//Arbritary Positioning
	m_prev = Button(getSharedAssets().backIcon, (int)(m_context.getWidth() * 0.30f), h, 0);
	m_next = Button(getSharedAssets().forwardIcon, (int)(m_context.getWidth() * 0.675f), h, 0);
	//Inital position of the text
	m_titlePos = { (m_context.getWidth() - getSharedAssets().openSansFont->getWidth(m_entries[m_entryIndex].m_name)) / 2
		, m_context.getHeight() - getSharedAssets().openSansFont->getLineHeight() * 2 };
}

RasterizerScene::~RasterizerScene()
{
}

#define DISTANCE 2.5f

void RasterizerScene::update() {
	m_prev.update();
	m_next.update();

	if (m_prev.isMouseOver() || m_next.isMouseOver()) {
		if (m_prev.isClicked())
			m_entryIndex--;

		if (m_next.isClicked())
			m_entryIndex++;

		m_entryIndex %= m_entries.size();

		//Update text so it is always in the center.
		m_titlePos = { (m_context.getWidth() - getSharedAssets().openSansFont->getWidth(m_entries[m_entryIndex].m_name)) / 2
			, m_context.getHeight() - getSharedAssets().openSansFont->getLineHeight() * 2 };
	}
	else {

		if (MCG::MouseDown)
		{
			int mouseX = 0;
			int mouseY = 0;
			SDL_GetMouseState(&mouseX, &mouseY);

			int diffX = mouseX - m_lastX;
			int diffY = mouseY - m_lastY;

			m_cameraYaw += diffX / 180.0f * M_PI;
			m_cameraPitch += diffY / 180.0f * M_PI;

			m_lastX = mouseX;
			m_lastY = mouseY;
		}
		else
		{
			SDL_GetMouseState(&m_lastX, &m_lastY);
		}
	}

}

void RasterizerScene::draw()
{
	m_context.lookAt(glm::cos(m_cameraYaw) * DISTANCE, 1.0f + (glm::cos(m_cameraPitch) * 5.0f), glm::sin(m_cameraYaw) * DISTANCE, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	m_context.m_lighting = true;
	m_context.m_model = m_entries[m_entryIndex].m_modelMatrix;
	m_entries[m_entryIndex].m_modelData.render(m_context);
	m_context.m_lighting = false;

	m_prev.draw(getContext());
	m_next.draw(getContext());

	getSharedAssets().openSansFont->drawText(getContext(), m_entries[m_entryIndex].m_name
		, m_titlePos.x, m_titlePos.y, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

	std::string hint = "Hold left mouse button and move mouse to rotate the model.";
	getSharedAssets().openSansFont->drawText(getContext(), hint
		, 5 , getContext().getHeight() - 20 , 0.75f, Filter::Bilinear , glm::vec4(0.4f, 0.4f, 0.4f, 1.0f));
}