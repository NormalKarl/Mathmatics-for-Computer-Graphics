#include "CurvesScene.h"
#include "Rasterizer.h"
#include "Surface.h"

CurvesScene::CurvesScene()
{
	setTitle("Curves");
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

void CurvesScene::draw() {
	Context& context = getContext();

	Vertex lastPoint = { 0,0, 0, 0.0f, 0.0f, 0.0f, 1.0f };

	for (int x = 1; x < 200; x++) {
		Vertex nextPoint = { (float) x, glm::round(EaseInOutQuad(x, 0, 200, 200)), 0, 0.0f, 0.0f, 0.0f, 1.0f };

		context.m_surface->setColourAt(nextPoint.m_position.x, nextPoint.m_position.y, nextPoint.m_colour);

		Rasterizer::DrawLine(context, lastPoint, nextPoint);

		lastPoint = nextPoint;
	}
}
