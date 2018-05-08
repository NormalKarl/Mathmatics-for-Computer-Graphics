#pragma once

#include <vector>
#include <string>

#include "Rasterizer.h"
#include "Texture.h"

struct BitmapChar {
	unsigned int id;
	unsigned int x;
	unsigned int y;
	unsigned int width;
	unsigned int height;
	int xoffset;
	int yoffset;
	int xadvance;
	unsigned int page;
	unsigned int channel;

	float uvX;
	float uvY;
	float uvWidth;
	float uvHeight;
};

struct Kerning {
	unsigned int first;
	unsigned int second;
	int amount;
};

//Supports bitmap fonts created by AngleCode's BMFont software.
//Requires XML export.
class BitmapFont
{
private:
	BitmapChar chars[256];
	std::vector<Kerning> kerning[256];
	Texture* texture;

	unsigned int lineHeight;
	unsigned int base;
	unsigned int scaleW;
	unsigned int scaleH;
	unsigned int pages;
	unsigned int size;
public:
	BitmapFont(const char* file, std::vector<const char*> pagePaths);
	~BitmapFont();

	inline int getLineHeight() { return lineHeight; }
	int getWidth(const std::string& text);
	void drawText(Context& context, std::string text, float x, float y, glm::vec4 col);
	void drawText(Context& context, std::string text, float x, float y, float scale = 1.0f, Filter filter = Filter::Point, glm::vec4 col = glm::vec4(1.0f));
};

