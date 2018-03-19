#pragma once

#include <GLM\glm.hpp>

enum class Filter
{
	Point,
	Bilinear
};

class Texture
{
public:
	int width;
	int height;
	int bytesPerPixel;
	unsigned char* data;
	Filter filter;

	inline Texture()
	{
		filter = Filter::Bilinear;
	}

	Texture(const char* filename);

	~Texture() {
		delete[] data;
	};

	glm::vec4 sample(glm::vec2 uv);
	glm::vec4 getPixelAt(int x, int y);
};