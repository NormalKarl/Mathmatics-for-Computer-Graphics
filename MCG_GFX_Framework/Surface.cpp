#include "Surface.h"
#include "MCG_GFX_Lib.h"
#include "Texture.h"
#include <SDL\SDL.h>

void GaussianBlur(Texture* texture);

Surface::Surface(const Viewport& _viewport)
{
	m_viewport = _viewport;
	m_frameBuffer = new FrameBuffer(m_viewport.m_width, m_viewport.m_height);
	m_frameFlagBuffer = new FlagBuffer(m_viewport.m_width, m_viewport.m_height);
	m_depthBuffer = new DepthBuffer(m_viewport.m_width, m_viewport.m_height);
	m_blur = false;
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


void Surface::writeToTexture(Texture* texture) {
	for (int x = 0; x < m_viewport.m_width; x++) {
		for (int y = 0; y < m_viewport.m_height; y++) {
			if (texture->inBounds(x, y)) {
				glm::vec4 dat = m_frameBuffer->get(x, y);
				texture->setPixelAt(x, y, (unsigned char)(dat.r * 255.0f), (unsigned char)(dat.g * 255.0f), (unsigned char)(dat.b * 255.0f), (unsigned char)(dat.a * 255.0f));
			}
		}
	}
}


Texture* Surface::getAsTexture() {
	Texture* texture = new Texture(m_frameBuffer->m_width, m_frameBuffer->m_height);

	writeToTexture(texture);

	return texture;
}

void Surface::draw()
{

	if (m_blur)
	{
		Texture* tex = getAsTexture();

		GaussianBlur(tex);

		for (int x = m_viewport.m_x; x < m_viewport.m_width; x++)
		{
			for (int y = m_viewport.m_y; y < m_viewport.m_height; y++)
			{
				Pixel p = tex->getRawPixel(x, y);

				if (p.a != 0) {
					MCG::DrawPixel({ x, m_viewport.m_height - y }, { p.r, p.g, p.b });
				}
			}
		}

		delete tex;
	}
	else
	{
		glm::vec4 clearColour = glm::vec4(m_clearColour, 1.0f);

		for (int x = m_viewport.m_x; x < m_viewport.m_width; x++)
		{
			for (int y = m_viewport.m_y; y < m_viewport.m_height; y++)
			{
				//If the pixel has been changed then draw it.
				if (m_frameFlagBuffer->get(x, y) == true)
				{
					glm::vec4 pixelCol = m_frameBuffer->get(x - m_viewport.m_x, y - m_viewport.m_y);
					float alpha = pixelCol.a;

					pixelCol *= alpha;
					pixelCol += clearColour * (1.0f - alpha);

					MCG::DrawPixel({ x, m_viewport.m_height - y }, pixelCol * 255.0f);
				}
			}
		}
	}
}

void Surface::setColourAt(int _x, int _y, glm::vec4 _colour)
{
	if (_colour.a == 0.0f)
		return;

	glm::vec4 currentColour = getColourAt(_x, _y);
	glm::vec4 newColour = _colour;

	//newColour = ((1.0f - _colour.a) * (currentColour.a * currentColour) + (_colour.a * _colour)) / currentColour.a;

	if (currentColour.a != 0.0f) {
		newColour.a = ((1.0f - _colour.a) * currentColour.a) + _colour.a;
		newColour.r = ((1.0f - _colour.a) * (currentColour.a * currentColour.r) + (_colour.a * _colour.r)) / currentColour.a;
		newColour.g = ((1.0f - _colour.a) * (currentColour.a * currentColour.g) + (_colour.a * _colour.g)) / currentColour.a;
		newColour.b = ((1.0f - _colour.a) * (currentColour.a * currentColour.b) + (_colour.a * _colour.b)) / currentColour.a;
	}

	m_frameBuffer->set(_x, _y, newColour);
	m_frameFlagBuffer->set(_x, _y, true);
}

void Surface::setClearColour(unsigned char r, unsigned char g, unsigned char b)
{
	m_clearColour = { (float)r / 255.0f, (float)g / 255.0f, (float)b / 255.0f };
}

void GaussianBlur(Texture* texture) {
	Texture lookUp = Texture(*texture);
	glm::mat3 kernel = glm::mat3(1.0f);

	for (int x = 0; x < texture->getWidth(); x++) {
		for (int y = 0; y < texture->getHeight(); y++) {
			Pixel pixel = lookUp.getRawPixel(x, y);
			int r = 0, g = 0, b = 0, a = pixel.a;
			int foundComponents = 0;

			for (int kX = -1; kX < 2; kX++) {
				for (int kY = -1; kY < 2; kY++) {
					if (lookUp.inBounds(x + kX, y + kY)) {
						Pixel pixel = lookUp.getRawPixel(x + kX, y + kY);
						r += pixel.r;
						g += pixel.g;
						b += pixel.b;
						foundComponents++;
					}
				}
			}

			r /= foundComponents;
			g /= foundComponents;
			b /= foundComponents;
			texture->setPixelAt(x, y, r, g, b, a);
		}
	}
}
