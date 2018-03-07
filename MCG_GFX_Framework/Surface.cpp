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

	for (int x = 0; x < m_viewport.width; x++)
	{
		for (int y = 0; y < m_viewport.height; y++)
		{
			//If the pixel has been changed then draw it.
			if (m_frameFlagBuffer->Get(x, y) == true)
			{
				glm::vec4 pixelCol = m_frameBuffer->Get(x, y);
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