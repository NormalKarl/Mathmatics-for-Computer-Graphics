#include "Surface.h"
#include "MCG_GFX_Lib.h"

Surface::Surface(const Viewport& _viewport)
{
	m_viewport = _viewport;
	m_frameBuffer = new FrameBuffer(m_viewport.width, m_viewport.height);
	m_frameFlagBuffer = new FlagBuffer(m_viewport.width, m_viewport.height);
	m_depthBuffer = new DepthBuffer(m_viewport.width, m_viewport.height);
}

Surface::~Surface()
{
	delete m_frameBuffer;
	delete m_frameFlagBuffer;
	delete m_depthBuffer;
}

void Surface::clear()
{
	m_frameBuffer->clear(glm::vec4(m_clearColour, 1.0f));
	m_frameFlagBuffer->clear(false);
	m_depthBuffer->clear(1.0f);
	MCG::SetBackground(m_clearColour * 255.0f);
}

void Surface::draw()
{
	glm::vec4 clearColour = glm::vec4(m_clearColour, 1.0f);

	for (int x = m_viewport.x; x < m_viewport.width; x++)
	{
		for (int y = m_viewport.y; y < m_viewport.height; y++)
		{
			//If the pixel has been changed then draw it.
			if (m_frameFlagBuffer->Get(x, y) == true)
			{
				glm::vec4 pixelCol = m_frameBuffer->Get(x - m_viewport.x, y - m_viewport.y);
				float alpha = pixelCol.a;

				pixelCol *= alpha;
				pixelCol += clearColour * (1.0f - alpha);

				MCG::DrawPixel({ x, m_viewport.height - y }, pixelCol * 255.0f);
			}
		}
	}
}

void Surface::setClearColour(unsigned char r, unsigned char g, unsigned char b)
{
	m_clearColour = { (float)r / 255.0f, (float)g / 255.0f, (float)b / 255.0f };
}

glm::vec3 Surface::getFlattenedPixel(int x, int y) {
	glm::vec4 pixelCol = m_frameBuffer->Get(x - m_viewport.x, y - m_viewport.y);
	float alpha = pixelCol.a;

	pixelCol *= alpha;
	pixelCol += glm::vec4(m_clearColour, 1.0f) * (1.0f - alpha);
	return glm::vec3(pixelCol);
}

float Surface::luma(int x, int y) {
	glm::vec4 clearColour = glm::vec4(m_clearColour, 1.0f);
	glm::vec4 pixelCol = m_frameBuffer->Get(x - m_viewport.x, y - m_viewport.y);
	float alpha = pixelCol.a;

	pixelCol *= alpha;
	pixelCol += clearColour * (1.0f - alpha);

	return glm::sqrt(glm::dot(glm::vec3(pixelCol), glm::vec3(0.299f, 0.587f, 0.114f)));
}

#define EDGE_THRESHOLD_MIN 0.0312f
#define EDGE_THRESHOLD_MAX 0.125f

//Implement http://blog.simonrodriguez.fr/articles/30-07-2016_implementing_fxaa.html;
glm::vec3 Surface::performFXAA(int x, int y) {
	float lumaCenter = luma(x, y);

	float lumaDown = luma(x, y + 1);
	float lumaUp = luma(x, y - 1);
	float lumaLeft = luma(x - 1, y);
	float lumaRight = luma(x + 1, y);

	float lumaMin = glm::min(lumaCenter, glm::min(glm::min(lumaDown, lumaUp), glm::min(lumaLeft, lumaRight)));
	float lumaMax = glm::max(lumaCenter, glm::max(glm::max(lumaDown, lumaUp), glm::max(lumaLeft, lumaRight)));

	float lumaRange = lumaMax - lumaMin;

	if (lumaRange < glm::max(EDGE_THRESHOLD_MIN, lumaMax * EDGE_THRESHOLD_MAX)) {
		return getFlattenedPixel(x, y);
	}




}