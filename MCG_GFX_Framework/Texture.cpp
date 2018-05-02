#include "Texture.h"
#include "stb_image.h"

#include <algorithm>

Texture::Texture()
{
	filter = Filter::Point;
	data = NULL;
}

Texture::Texture(const Texture& texture) : width(texture.width), height(texture.height), filter(texture.filter), bytesPerPixel(texture.bytesPerPixel) {
	int size = width * height * bytesPerPixel;
	data = new unsigned char[size];
	std::copy(texture.data, texture.data + size, data);
}

Texture::Texture(int _width, int _height, Filter _filter) : width(_width), height(_height), filter(_filter) {
	bytesPerPixel = 4;
	this->data = new unsigned char[width * height * bytesPerPixel];
}

Texture::Texture(const char* filename, Filter filter)
{
	data = stbi_load(filename, &width, &height, &bytesPerPixel, STBI_rgb_alpha);
	this->filter = filter;
}

Texture::~Texture()
{
	delete data;
}

glm::vec4 Texture::sample(glm::vec2 uv)
{
	//float x = glm::clamp(uv.x, 0.0f, 1.0f);
	//float y = glm::clamp(uv.y, 0.0f, 1.0f);

	double approxPosX = (double)uv.x * double(width);
	double approxPosY = (double)uv.y * double(height);

	int pixelX = static_cast<int>(glm::floor(approxPosX));
	int pixelY = static_cast<int>(glm::floor(approxPosY));
	//printf("Pixel W, %i, H %i, X %i, Y %i\n, U %f, V %f", width, height, pixelX, pixelY, uv.x, uv.y);

	glm::vec4 pixel = getPixelAt(pixelX, pixelY);

	if (filter == Filter::Bilinear)
	{
		float unitPosX = (approxPosX - (float)pixelX);
		float unitPosY = (approxPosY - (float)pixelY);
		float unitPosXInv = 1.0f - unitPosX;
		float unitPosYInv = 1.0f - unitPosY;

		glm::vec4 right = getPixelAt(pixelX + 1, pixelY);
		glm::vec4 down = getPixelAt(pixelX, pixelY + 1);
		glm::vec4 rightDown = getPixelAt(pixelX + 1, pixelY + 1);

		pixel = (pixel * unitPosXInv + right * unitPosX) * unitPosYInv + (down * unitPosXInv + rightDown * unitPosX) * unitPosY;
	}

	if (bytesPerPixel == 3)
		pixel.a = 1.0f;

	return pixel;
}

glm::vec4 Texture::getPixelAt(int x, int y)
{
	x = glm::clamp(x, 0, width - 1);
	y = glm::clamp(y, 0, height - 1);
	//Problem is here somewhere
	unsigned char dat[4] = { 0 };
	memcpy_s(dat, bytesPerPixel, &data[((y * width) + x) * bytesPerPixel], bytesPerPixel);

	return glm::vec4((float)dat[0] / 255.0f, (float)dat[1] / 255.0f, (float)dat[2] / 255.0f, (float)dat[3] / 255.0f);
}

void Texture::setPixelAt(int x, int y, unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
	data[(((y * width) + x) * bytesPerPixel) + 0] = r;
	data[(((y * width) + x) * bytesPerPixel) + 1] = g;
	data[(((y * width) + x) * bytesPerPixel) + 2] = b;
	data[(((y * width) + x) * bytesPerPixel) + 3] = a;
}

bool Texture::inBounds(int x, int y) {
	return x >= 0 && x < width && y >= 0 && y < height;
}

Pixel Texture::getRawPixel(int x, int y) {
	Pixel p;

	p.r = data[(((y * width) + x) * bytesPerPixel) + 0];
	p.g = data[(((y * width) + x) * bytesPerPixel) + 1];
	p.b = data[(((y * width) + x) * bytesPerPixel) + 2];
	p.a = data[(((y * width) + x) * bytesPerPixel) + 3];
	return p;
	//return *((Pixel*)data[((y * width) + x) * bytesPerPixel]);
}