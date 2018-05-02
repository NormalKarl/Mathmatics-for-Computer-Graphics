#include <cmath>
#include <ctime>
#include <vector>
#include <algorithm>
#include <thread>
#include "MCG_GFX_Lib.h"
#include "Rasterizer.h"
#include "stb_image.h"
#include "Scene.h"
#include "PrimitivesScene.h"
#include "RasterizerScene.h"
#include "RaytracerScene.h"
#include "MandlebrotScene.h"
#include "BitmapFont.h"
#include "Surface.h"
#include "MenuScene.h"
#include "Defs.h"

#include <SDL/SDL.h>

int main(int argc, char *argv[])
{
	Surface* surface = new Surface({ 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT });
	SceneManager* sceneManager = new SceneManager(surface);

	MenuScene* menuScene = new MenuScene();
	PrimitivesScene* primitivesScene = new PrimitivesScene();
	RasterizerScene* rasterizerScene = new RasterizerScene();
	RaytracerScene* raytracerScene = new RaytracerScene();
	MandlebrotScene* mandlebrotScene = new MandlebrotScene();

	sceneManager->addScene(menuScene);
	sceneManager->addScene(primitivesScene);
	sceneManager->addScene(rasterizerScene);
	sceneManager->addScene(raytracerScene);
	sceneManager->addScene(mandlebrotScene);
	sceneManager->goToScene(menuScene);

	if (!MCG::Init({ SCREEN_WIDTH, SCREEN_HEIGHT }))
		return -1;

	//surface->setClearColour((unsigned char)246, 114, 128);
	surface->setClearColour((unsigned char)255, 255, 255);
	
	while (MCG::ProcessFrame())
	{
		sceneManager->update();
		surface->clear();
		sceneManager->draw();
		surface->draw();
	}

	return 0;
}
