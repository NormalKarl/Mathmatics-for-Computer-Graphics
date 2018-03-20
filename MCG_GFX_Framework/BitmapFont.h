#pragma once

#include <vector>
#include <string>

#include "Texture.h"

class Rasterizer;

//I have chosen to define a ushort for the BitmapChar because its very unlikly that it will be any bigger.
typedef unsigned short ushort;

struct BitmapChar {
	ushort id;
	ushort x;
	ushort y;
	ushort width;
	ushort height;
	ushort xoffset;
	ushort yoffset;
	ushort xadvance;
	ushort yadvance;
	ushort page;
	ushort channel;

	float uvX;
	float uvY;
	float uvWidth;
	float uvHeight;
};

//Supports bitmap fonts created by AngleCode's BMFont software.
//Requires XML export.
class BitmapFont
{
private:
	BitmapChar chars[256];
	Texture texture;

	ushort lineHeight;
	ushort base;
public:
	BitmapFont(const char* file, std::vector<const char*> pagePaths);
	~BitmapFont();


	void drawText(Rasterizer* rasterizer, std::string text, float x, float y);
};

