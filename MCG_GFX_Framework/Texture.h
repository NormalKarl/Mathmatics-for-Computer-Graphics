#pragma once

#include <GLM\glm.hpp>

enum class Filter
{
	Point,
	Bilinear
};

struct Pixel {
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
};

class Texture
{
public:
	int width;
	int height;
	int bytesPerPixel;
	unsigned char* data;
	Filter filter;

	Texture();
	Texture(const Texture& texture);
	Texture(int _width, int _height, Filter _filter = Filter::Point);
	Texture(const char* filename, Filter filter = Filter::Point);

	~Texture();

	glm::vec4 sample(glm::vec2 uv);
	glm::vec4 getPixelAt(int x, int y);
	void setPixelAt(int x, int y, unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	Pixel getRawPixel(int x, int y);

	bool inBounds(int x, int y);

	inline int getWidth() {
		return width;
	}

	inline int getHeight() {
		return height;
	}

	inline float getWidthF() {
		return (float)width;
	}

	inline float getHeightF() {
		return (float)height;
	}
	

};