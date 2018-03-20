#pragma once

#include <vector>
#include <string>

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
};

class BitmapFont
{
private:
	BitmapChar chars[256];
	Texture texture;
public:
	BitmapFont(const char* file);
	~BitmapFont();

	void drawText(Rasterizer* rasterizer, std::string text, float x, float y);
};

