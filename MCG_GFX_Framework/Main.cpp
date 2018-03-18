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

inline glm::vec4 intersect(glm::vec4 v0, glm::vec4 v1, glm::vec2 c0, glm::vec2 c1) {
	float x = (c0.x * c1.y - c0.y * c1.x) * (v0.x - v1.x) - (c0.x - c1.x) * (v0.x * v1.y - v0.y * v1.x);
	float y = (c0.x * c1.y - c0.y * c1.x) * (v0.y - v1.y) - (c0.y - c1.y) * (v0.x * v1.y - v0.y * v1.x);

	float div = (c0.x - c1.x) * (v0.y - v1.y) - (c0.y - c1.y) * (v0.x - v1.x);

	x /= div;
	y /= div;

	return glm::vec4(x, y, v0.z, v0.w);
}


//Sutherland-Hodgeman Clipping Algorithm
void edgeClip(std::vector<glm::vec4>& vertices, std::vector<glm::vec2> clips) {
	//Define 2 vectors for both the current iteration and the new vertices.
	std::vector<glm::vec4> iteration = vertices;
	std::vector<glm::vec4> newVertices;

	for (int c = 0; c < clips.size(); c++) {
		glm::vec2 c1 = clips[c];
		glm::vec2 c2 = clips[(c + 1) % clips.size()];
		
		for (int v = 0; v < iteration.size(); v++) {
			glm::vec4 v1 = iteration[v];
			glm::vec4 v2 = iteration[(v + 1) % iteration.size()];

			//Get the sign's of the side in which the point resides.
			int sign1 = getSide(v1, c1, c2);
			int sign2 = getSide(v2, c1, c2);

			//First point is inside the clipping plane.
			if (sign1 >= 0) {
				//Add current point.
				newVertices.push_back(v1);

				//Second point is outside the clipping plane.
				if (sign2 < 0) {
					//Add intersected point.
					newVertices.push_back(intersect(v1, v2, c1, c2));
				}
			}
			//First point is outside the clipping plane and second point is inside.
			else if (sign2 >= 0) {
				//Add intersected point.
				newVertices.push_back(intersect(v1, v2, c1, c2));
			}
		}

		//Set new iteration to clipped vertices and clear the new vertex point.
		iteration = newVertices;
		newVertices.clear();
	}

	//Allocated the new clipped vertices to the referenced input.
	vertices = iteration;
}

//Ear-Clipping Algorithm
void earClip(std::vector<glm::vec4>& vertices) {

}

int main(int argc, char *argv[])
{
	/*Surface* surface = new Surface({ 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT });
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
	}*/

	//std::vector<glm::vec4> vertices = { { 0.0f, 0.0f, 1.0f, 1.0f },{ 0.0f,1.5f, 1.0f, 1.0f },{ 1.5f,0.0f, 1.0f, 1.0f } };
	//clip(vertices, { { 0.0f,0.0f }, { 0.0f,1.0f }  ,{ 1.0f,1.0f },{ 1.0f,0.0f } });


	//glm::vec4 i = intersect({ 0.0f, 0.0f,1.0f, 0.0f }, {1.0f, 1.0f, 1.0f, 0.0f}, { 5.0f, 0.0f }, { 5.0f, 10.0f });

	//return 0;
}
