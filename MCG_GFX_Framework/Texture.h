#pragma once

#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <GLM\glm.hpp>

//Filters used for Texture
enum class Filter
{
	Point,
	Bilinear
};

//Used to look up a single pixel in Texture
struct Pixel {
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
};

/*
Texture is used for storing the raw RGBA data of an image with use of stb_image.h .
*/
class Texture
{
private:
	int width;
	int height;
	int bytesPerPixel;
	unsigned char* data;
	Filter filter;
public:
	Texture();
	Texture(const Texture& texture);
	Texture(int _width, int _height, Filter _filter = Filter::Point);
	Texture(const char* filename, Filter filter = Filter::Point);
	~Texture();
	//Sample this texture.
	glm::vec4 sample(glm::vec2 uv);
	//Returns the pixel as a 0-1 glm::vec4.
	glm::vec4 getPixelAt(int x, int y);
	void setPixelAt(int x, int y, unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	//Return the pixel as Pixel
	Pixel getRawPixel(int x, int y);
	bool inBounds(int x, int y);
	inline int getWidth() { return width; }
	inline int getHeight() { return height; }
	inline float getWidthF() { return (float)width; }
	inline float getHeightF() { return (float)height; }
	inline Filter getFilter() { return filter; }
	inline void setFilter(Filter _filter) { filter = _filter; }
};

#endif