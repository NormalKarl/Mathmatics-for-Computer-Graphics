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
#include "BitmapFont.h"

#include <SDL/SDL.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

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
	sceneManager->goToScene(raytracerScene);

	if (!MCG::Init({ SCREEN_WIDTH, SCREEN_HEIGHT }))
		return -1;

	float timer = 0.0f;

	surface->setClearColour((unsigned char)163, 198, 255);

	while (MCG::ProcessFrame())
	{
		timer += 1.0f / 60.0f;

		sceneManager->update();
		surface->clear();
		sceneManager->draw();
		surface->draw();
	}

	return 0;
}
