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

	pointA = { centerX - 100, centerY - 100 };
	pointB = { centerX, centerY - 100 };
	pointC = { centerX, centerY + 100 };
	pointD = { centerX + 100, centerY + 100 };
	focusedPoint = NULL;

	//This is just some arbritary placement for the buttons.
	float h = getContext().getHeight() - getSharedAssets().openSansFont->getLineHeight() * 2;
	m_titlePos = glm::vec2((SCREEN_WIDTH - getSharedAssets().openSansFont->getWidth("Bezier Curve")) / 2, h);

	//Arbritary Positioning
	m_prev = Button(getSharedAssets().backIcon, (int)(getContext().getWidth() * 0.30f), h, 0);
	m_next = Button(getSharedAssets().forwardIcon, (int)(getContext().getWidth() * 0.675f), h, 0);
	m_titlePos = glm::vec2((SCREEN_WIDTH - getSharedAssets().openSansFont->getWidth("Bezier Curve")) / 2, h + 6);
	m_exampleNo = 0;
}

float Linear(float t, float b, float c, float d) {
	return b + ((c - b) * t);
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

	m_prev.update();
	m_next.update();

	m_titlePos.x = (SCREEN_WIDTH - getSharedAssets().openSansFont->getWidth("Bezier Curve")) / 2;

	if (m_prev.isMouseOver() || m_next.isMouseOver()) {
		if (m_prev.isClicked()) {
			m_exampleNo--;
		}
		else if (m_next.isClicked()) {
			m_exampleNo++;
		}

		m_exampleNo %= 3;
		printf("%i\n", m_exampleNo);
	}
	else {
		//Code to check for points
		if (MCG::MouseDown) {
			if (inBounds(pointB, mouseX, mouseY)) {
				focusedPoint = &pointB;
			}
			else if (inBounds(pointC, mouseX, mouseY)) {
				focusedPoint = &pointC;
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
}

void CurvesScene::draw() {
	Context& context = getContext();

	Rasterizer::DrawImage(getContext(), getSharedAssets().axisBackground
		, ((context.getWidth() - getSharedAssets().axisBackground->getWidth()) / 2)
		, ((context.getHeight() - getSharedAssets().axisBackground->getHeight()) / 2));

	m_prev.draw(context);
	m_next.draw(context);

	std::string title;

	switch (m_exampleNo) {
	case 0:
		title = "Bezier Curve";
		for (float x = 0; x <= 1.0f; x += 0.001f) {
			glm::vec2 p = point(x, pointA, pointB, pointC, pointD);
			Rasterizer::DrawPoint(context, { p.x, p.y, 0, 0.0f, 0.0f, 0.0f, 1.0f });
		}

		Rasterizer::DrawLine(getContext(), { pointA.x, pointA.y, 0.0f, 0.7f, 0.7f, 0.7f, 1.0f }, { pointB.x, pointB.y, 0.0f, 0.7f, 0.7f, 0.7f, 1.0f });
		Rasterizer::DrawLine(getContext(), { pointC.x, pointC.y, 0.0f, 0.7f, 0.7f, 0.7f, 1.0f }, { pointD.x, pointD.y, 0.0f, 0.7f, 0.7f, 0.7f, 1.0f });

		Rasterizer::DrawImage(getContext(), getSharedAssets().holdIcon, pointB.x - 6, pointB.y - 6);
		Rasterizer::DrawImage(getContext(), getSharedAssets().holdIcon, pointC.x - 6, pointC.y - 6);

		break;
	case 1:
		title = "Ease In Quad";

		for (float x = 0; x <= 1.0f; x += 0.001f) {
			Rasterizer::DrawPoint(context, { pointA.x + (x * 200.0f), pointA.y + (EaseInQuad(x, 0, 200, 1.0f)), 0, 0.0f, 0.0f, 0.0f, 1.0f });
		}

		break;
	case 2:
		title = "Ease In Out Quad";

		for (float x = 0; x <= 1.0f; x += 0.001f) {
			Rasterizer::DrawPoint(context, { pointA.x + (x * 200.0f), pointA.y + (EaseInOutQuad(x, 0, 200, 1.0f)), 0, 0.0f, 0.0f, 0.0f, 1.0f });
		}

		break;
	}

	Rasterizer::DrawImage(getContext(), getSharedAssets().staticIcon, pointA.x - 6, pointA.y - 6);
	Rasterizer::DrawImage(getContext(), getSharedAssets().staticIcon, pointD.x - 6, pointD.y - 6);

	m_titlePos.x = (SCREEN_WIDTH - getSharedAssets().openSansFont->getWidth(title)) / 2;
	getSharedAssets().openSansFont->drawText(getContext(), title, m_titlePos.x, m_titlePos.y, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

}
