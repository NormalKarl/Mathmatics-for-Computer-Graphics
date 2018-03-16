#include <cmath>
#include <ctime>
#include <vector>
#include <algorithm>
#include <thread>
#include "MCG_GFX_Lib.h"
#include "Rasterizer.h"
#include "stb_image.h"
#include "Scene.h"
#include "RasterizerScene.h"
#include "RaytracerScene.h"
#include "MandlebrotScene.h"

#include <SDL/SDL.h>

#define SCREEN_WIDTH 960
#define SCREEN_HEIGHT 540

inline int getSide(glm::vec2 p, glm::vec2 line0, glm::vec2 line1) {
	return glm::sign((p.x - line0.x) * (line1.y - line0.y) - (p.y - line0.y) * (line1.x - line0.x));
}

inline glm::vec4 intersect(glm::vec2 c0, glm::vec2 c1, glm::vec4 v0, glm::vec4 v1) {
	float x = (c0.x * c1.y - c0.y * c1.x) * (v0.x - v1.x) - (c0.x - c1.x) * (v0.x * v1.y - v0.y * v1.x);
	float y = (c0.x * c1.y - c0.y * c1.x) * (v0.y - v1.y) - (c0.y - c1.y) * (v0.x * v1.y - v0.y * v1.x);

	float div = (c0.x - c1.x) * (v0.y - v1.y) - (c0.y - c1.y) * (v0.x - v1.x);

	return glm::vec4(x / div, y / div, v0.z, v0.w);
}

void clip(std::vector<glm::vec4>& vertices, std::vector<glm::vec2> clips)
{
	std::vector<glm::vec4> newVertices;

	for (int c = 0; c < clips.size(); c++) {
		//Clip points for current and the next to create a line.
		glm::vec2 c0 = clips[c];
		glm::vec2 c1 = clips[(c + 1) % clips.size()];

		glm::vec4 v1 = vertices[vertices.size() - 1];

		for (int v = 0; v < vertices.size(); v++)
		{
			glm::vec4 v0 = vertices[v];

			int E = getSide(v0, c0, c1);
			int S = getSide(v1, c0, c1);

			if (E >= 0) {
				if (S < 0) {
					newVertices.push_back(intersect(c0, c1, v0, v1));
				}
				newVertices.push_back(v0);
			}
			else if (S >= 0) {
				newVertices.push_back(intersect(c0, c1, v0, v1));
			}

			v1 = v0;
		}

	}

	vertices = newVertices;
}

int main(int argc, char *argv[])
{
	Surface* surface = new Surface({ 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT });
	SceneManager* sceneManager = new SceneManager(surface);

	RasterizerScene* rasterizerScene = new RasterizerScene();
	RaytracerScene* raytracerScene = new RaytracerScene();
	MandlebrotScene* mandlebrotScene = new MandlebrotScene();

	sceneManager->addScene(rasterizerScene);
	sceneManager->addScene(raytracerScene);
	sceneManager->addScene(mandlebrotScene);
	sceneManager->goToScene(1);

	if (!MCG::Init({ SCREEN_WIDTH, SCREEN_HEIGHT }))
		return -1;

	float timer = 0.0f;

	surface->setClearColour((unsigned char)255, 255, 255);

	while (MCG::ProcessFrame())
	{
		timer += 1.0f / 60.0f;

		sceneManager->update();
		surface->clear();
		sceneManager->draw();
		surface->draw();
	}

	std::vector<glm::vec4> vertices = { { 100.0f, 150.0f, 1.0f, 1.0f },{ 200.0f,250.0f, 1.0f, 1.0f },{ 300.0f,200.0f, 1.0f, 1.0f } };
	clip(vertices, { { 150.0f,150.0f },{ 150.0f,200.0f },{ 200.0f,200.0f },{ 200.0f,150.0f } });

	return 0;
}
