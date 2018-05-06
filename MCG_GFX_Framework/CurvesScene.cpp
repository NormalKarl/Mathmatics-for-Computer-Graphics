#include "CurvesScene.h"
#include "Rasterizer.h"
#include "Surface.h"
#include "Defs.h"
#include "MCG_GFX_Lib.h"

#include <SDL\SDL.h>

CurvesScene::CurvesScene()
{
	setTitle("Curves");

	float centerX = SCREEN_WIDTH / 2;
	float centerY = SCREEN_HEIGHT / 2;

	bA = { centerX - 100, centerY - 100 };
	bB = { centerX, centerY - 100 };
	bC = { centerX, centerY + 100 };
	bD = { centerX + 100, centerY + 100 };
	focusedPoint = NULL;
}

CurvesScene::~CurvesScene()
{

}

float EaseInQuad(float t, float b, float c, float d) {
	return c * (t /= d)*t + b;
};

float EaseInOutQuad(float t, float b, float c, float d) {
if ((t /= d / 2) < 1) return c / 2 * t*t + b;
return -c / 2 * ((--t)*(t - 2) - 1) + b;
};

glm::vec2 point(float t, glm::vec2 a, glm::vec2 b, glm::vec2 c, glm::vec2 d) {

	t = glm::clamp(t, 0.0f, 1.0f);

	glm::vec2 na = powf((1.0f - t), 3) * a;
	glm::vec2 nb = 3 * t * powf((1 - t), 2) * b;
	glm::vec2 nc = 3 * powf(t, 2) * (1 - t) * c;
	glm::vec2 nd = powf(t, 3) * d;
	return na + nb + nc + nd;
}

bool inBounds(glm::vec2 p, int x, int y) {
	return (x >= p.x - 5 && y >= p.y - 5 && x < p.x + 5 && y < p.y + 5);
}

void CurvesScene::update() {
	int mouseX, mouseY;

	SDL_GetMouseState(&mouseX, &mouseY);

	if (MCG::MouseDown) {
		if (inBounds(bA, mouseX, mouseY)) {
			focusedPoint = &bA;
		} else if (inBounds(bB, mouseX, mouseY)) {
			focusedPoint = &bB;
		} else if (inBounds(bC, mouseX, mouseY)) {
			focusedPoint = &bC;
		} else if (inBounds(bD, mouseX, mouseY)) {
			focusedPoint = &bD;
		}
	}
	else {
		focusedPoint = NULL;
	}

	if (focusedPoint != NULL) {
		focusedPoint->x = mouseX;
		focusedPoint->y = mouseY;
	}
}

void CurvesScene::draw() {
	Context& context = getContext();

	Vertex lastPoint = { 0,0, 0, 0.0f, 0.0f, 0.0f, 1.0f };

	for (float x = 0; x < 1.0f; x += 0.001f) {
		glm::vec2 p = point(x, bA, bB, bC, bD);

		//p.y = bA.y + ((bD.y - bA.y) - (p.y - bA.y));

		//Vertex nextPoint = { (float) x, glm::round(EaseInOutQuad(x, 0, 200, 200)), 0, 0.0f, 0.0f, 0.0f, 1.0f };
		Vertex nextPoint = { p.x, p.y, 0, 0.0f, 0.0f, 0.0f, 1.0f };

		Rasterizer::DrawPoint(context, nextPoint);
		Rasterizer::DrawLine(context, lastPoint, nextPoint);

		lastPoint = nextPoint;
	}

	Rasterizer::DrawLine(getContext(), { bA.x, bA.y, 0.0f, 0.7f, 0.7f, 0.7f, 1.0f }, { bB.x, bB.y, 0.0f, 0.7f, 0.7f, 0.7f, 1.0f });
	Rasterizer::DrawLine(getContext(), { bC.x, bC.y, 0.0f, 0.7f, 0.7f, 0.7f, 1.0f }, { bD.x, bD.y, 0.0f, 0.7f, 0.7f, 0.7f, 1.0f });

	Rasterizer::DrawImage(getContext(), getSharedAssets().holdIcon, bA.x - 6, bA.y - 6);
	Rasterizer::DrawImage(getContext(), getSharedAssets().holdIcon, bB.x - 6, bB.y - 6);
	Rasterizer::DrawImage(getContext(), getSharedAssets().holdIcon, bC.x - 6, bC.y - 6);
	Rasterizer::DrawImage(getContext(), getSharedAssets().holdIcon, bD.x - 6, bD.y - 6);

	getSharedAssets().openSansFont->drawText(getContext(), "Bezier Curve"
		, (SCREEN_WIDTH - getSharedAssets().openSansFont->getWidth("Bezier Curve")) / 2, 400
		, 1.0f, Filter::Point, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
}
