#include "Texture.h"
#include "stb_image.h"

Texture::Texture()
{
	filter = Filter::Bilinear;
	data = NULL;
}

Texture::Texture(const char* filename)
{
	data = stbi_load(filename, &width, &height, &bytesPerPixel, STBI_rgb_alpha);
	filter = Filter::Point;
}

Texture::~Texture()
{
	//stbi_image_free(data);
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