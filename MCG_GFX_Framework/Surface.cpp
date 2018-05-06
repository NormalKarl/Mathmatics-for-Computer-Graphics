#include "Surface.h"
#include "MCG_GFX_Lib.h"
#include "Texture.h"
#include <SDL\SDL.h>

void GaussianBlur(Texture* texture);
void Bloom(Texture* texture);

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


void Surface::writeToTexture(Texture* texture) {
	for (int x = 0; x < m_viewport.width; x++) {
		for (int y = 0; y < m_viewport.height; y++) {
			if (texture->inBounds(x, y)) {
				glm::vec4 dat = m_frameBuffer->get(x, y);
				texture->setPixelAt(x, y, (unsigned char)(dat.r * 255.0f), (unsigned char)(dat.g * 255.0f), (unsigned char)(dat.b * 255.0f), (unsigned char)(dat.a * 255.0f));
			}
		}
	}
}


Texture* Surface::getAsTexture() {
	Texture* texture = new Texture(m_frameBuffer->width, m_frameBuffer->height);

	writeToTexture(texture);

	return texture;
}

void Surface::draw()
{
	/*{
		Texture* tex = getAsTexture();

		Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
		int shift = (req_format == STBI_rgb) ? 8 : 0;
		rmask = 0xff000000 >> shift;
		gmask = 0x00ff0000 >> shift;
		bmask = 0x0000ff00 >> shift;
		amask = 0x000000ff >> shift;
#else // little endian, like x86
		rmask = 0x000000ff;
		gmask = 0x0000ff00;
		bmask = 0x00ff0000;
		amask = 0xff000000;
#endif

		int depth, pitch;
		depth = 32;
		pitch = 4 * tex->width;
		SDL_Surface* surface = SDL_CreateRGBSurfaceFrom(tex->data, tex->width, tex->height, depth, pitch, rmask, gmask, bmask, amask);
		MCG::DrawSurface(surface);
		delete tex;
	}*/

	glm::vec4 clearColour = glm::vec4(m_clearColour, 1.0f);

	for (int x = m_viewport.x; x < m_viewport.width; x++)
	{
		for (int y = m_viewport.y; y < m_viewport.height; y++)
		{
			//If the pixel has been changed then draw it.
			if (m_frameFlagBuffer->get(x, y) == true)
			{
				glm::vec4 pixelCol = m_frameBuffer->get(x - m_viewport.x, y - m_viewport.y);
				float alpha = pixelCol.a;

				pixelCol *= alpha;
				pixelCol += clearColour * (1.0f - alpha);

				MCG::DrawPixel({ x, m_viewport.height - y }, pixelCol * 255.0f);
			}
		}
	}


	/*Texture* tex = getAsTexture();
	Bloom(tex);

	for (int x = m_viewport.x; x < m_viewport.width; x++)
	{
		for (int y = m_viewport.y; y < m_viewport.height; y++)
		{
			Pixel p = tex->getRawPixel(x, y);
			MCG::DrawPixel({ x, m_viewport.height - y }, {p.r, p.g, p.b});
		}
	}

	delete tex;*/
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

glm::vec3 Surface::getFlattenedPixel(int x, int y) {
	glm::vec4 pixelCol = m_frameBuffer->get(x - m_viewport.x, y - m_viewport.y);
	float alpha = pixelCol.a;

	pixelCol *= alpha;
	pixelCol += glm::vec4(m_clearColour, 1.0f) * (1.0f - alpha);
	return glm::vec3(pixelCol);
}

float Surface::luma(int x, int y) {
	glm::vec4 clearColour = glm::vec4(m_clearColour, 1.0f);
	glm::vec4 pixelCol = m_frameBuffer->get(x - m_viewport.x, y - m_viewport.y);
	float alpha = pixelCol.a;

	pixelCol *= alpha;
	pixelCol += clearColour * (1.0f - alpha);

	return glm::sqrt(glm::dot(glm::vec3(pixelCol), glm::vec3(0.299f, 0.587f, 0.114f)));
}

void GaussianBlur(Texture* texture) {
	Texture lookUp = Texture(*texture);
	glm::mat3 kernel = glm::mat3(1.0f);

	for (int x = 0; x < texture->width; x++) {
		for (int y = 0; y < texture->height; y++) {
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

void Bloom(Texture* texture) {
	//Make a copy of the current texture.
	Texture* bloomTexture = new Texture(*texture);

	for (int x = 0; x < texture->width; x++) {
		for (int y = 0; y < texture->height; y++) {
			Pixel tp = texture->getRawPixel(x, y);
			Pixel bp = bloomTexture->getRawPixel(x, y);

			if (tp.r >= 200 && tp.g >= 200 && tp.b >= 200)
				texture->setPixelAt(x, y, 0, 0, 0, 0);

			if (bp.r < 200 && bp.g < 200 && bp.b < 200)
				bloomTexture->setPixelAt(x, y, 0, 0, 0, 0);
		}
	}

	GaussianBlur(bloomTexture);


	for (int x = 0; x < texture->width; x++) {
		for (int y = 0; y < texture->height; y++) {
			Pixel tp = texture->getRawPixel(x, y);
			Pixel bp = bloomTexture->getRawPixel(x, y);

			int r = (((int)tp.r + (int)bp.r));
			int g = (((int)tp.g + (int)bp.g));
			int b = (((int)tp.b + (int)bp.b));

			int a = (((int)tp.a + (int)bp.a));

			float tpA = (float)tp.a / 255.0f;
			float bpA = (float)bp.a / 255.0f;

			r = ((1.0f - tpA) * (bpA * bp.r) + (tpA * tp.r)) / bpA;
			g = ((1.0f - tpA) * (bpA * bp.g) + (tpA * tp.g)) / bpA;
			b = ((1.0f - tpA) * (bpA * bp.b) + (tpA * tp.b)) / bpA;
			a = ((1.0f - tpA) * bpA) + tpA;

			texture->setPixelAt(x, y, r, g, b, a);
		}
	}

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