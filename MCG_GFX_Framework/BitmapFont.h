#pragma once

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
public:
	BitmapFont(const char* file);
	~BitmapFont();
};

