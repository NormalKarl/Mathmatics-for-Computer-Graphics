#include <cmath>
#include <ctime>
#include <vector>
#include <algorithm>
#include <thread>
#include <SDL/SDL.h>

#include "MCG_GFX_Lib.h"

#include "Surface.h"
#include "Defs.h"

#include "Scene.h"
#include "MenuScene.h"
#include "PrimitivesScene.h"
#include "RasterizerScene.h"
#include "RaytracerScene.h"
#include "FractalsScene.h"
#include "CurvesScene.h"

int main(int argc, char *argv[])
{
	Surface* surface = new Surface({ 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT });
	SceneManager* sceneManager = new SceneManager(surface);
	MenuScene* menuScene = new MenuScene();
	PrimitivesScene* primitivesScene = new PrimitivesScene();
	RaytracerScene* raytracerScene = new RaytracerScene();
	RasterizerScene* rasterizerScene = new RasterizerScene();
	CurvesScene* curvesScene = new CurvesScene();
	FractalsScene* fractalsScene = new FractalsScene();

	sceneManager->addScene(menuScene);
	sceneManager->addScene(primitivesScene);
	sceneManager->addScene(raytracerScene);
	sceneManager->addScene(rasterizerScene);
	sceneManager->addScene(curvesScene);
	sceneManager->addScene(fractalsScene);
	sceneManager->goToScene(menuScene);

	if (!MCG::Init({ SCREEN_WIDTH, SCREEN_HEIGHT }))
		return -1;

	surface->setClearColour((unsigned char)255, 255, 255);
	
	while (MCG::ProcessFrame())
	{
		sceneManager->update();
		surface->clear();
		sceneManager->draw();
		surface->draw();
	}

	delete primitivesScene;
	delete raytracerScene;
	delete rasterizerScene;
	delete curvesScene;
	delete fractalsScene;
	delete sceneManager;
	delete surface;
	return 0;
}
