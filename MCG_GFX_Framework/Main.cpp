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


//Ear-Clipping Algorithm
//n = vertices. triangles = n - 2;
void earClip(std::vector<glm::vec4>& vertices) {

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
	sceneManager->goToScene(rasterizerScene);

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

	//std::vector<glm::vec4> vertices = { { 0.0f, 0.0f, 1.0f, 1.0f },{ 0.0f,1.5f, 1.0f, 1.0f },{ 1.5f,0.0f, 1.0f, 1.0f } };
	//clip(vertices, { { 0.0f,0.0f }, { 0.0f,1.0f }  ,{ 1.0f,1.0f },{ 1.0f,0.0f } });


	//glm::vec4 i = intersect({ 0.0f, 0.0f,1.0f, 0.0f }, {1.0f, 1.0f, 1.0f, 0.0f}, { 5.0f, 0.0f }, { 5.0f, 10.0f });

	//return 0;
}
