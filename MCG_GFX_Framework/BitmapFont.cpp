#include "BitmapFont.h"

#include <fstream>
#include <sstream>
#include <string>

#include "tinyxml2.h"
#include "Rasterizer.h"

BitmapFont::BitmapFont(const char* filename) {
	std::fill_n((char*)chars, sizeof(BitmapChar) * 256, NULL);


	tinyxml2::XMLDocument doc;
	doc.LoadFile(filename);

	tinyxml2::XMLElement* e = doc.FirstChildElement("font")->FirstChildElement("chars");

	for (tinyxml2::XMLElement* c = (tinyxml2::XMLElement*)e->FirstChild(); c != NULL; c = (tinyxml2::XMLElement*)c->NextSibling()) {
		BitmapChar ch = { 0 };

		ch.id = c->IntAttribute("id");
		ch.x = c->IntAttribute("x");
		ch.y = c->IntAttribute("y");
		ch.width = c->IntAttribute("width");
		ch.height = c->IntAttribute("height");
		ch.xoffset = c->IntAttribute("xoffset");
		ch.yoffset = c->IntAttribute("yoffset");
		ch.xadvance = c->IntAttribute("xadvance");
		ch.page = c->IntAttribute("page");
		ch.channel = c->IntAttribute("chnl");

		chars[ch.id] = ch;
	}

	return;
}


BitmapFont::~BitmapFont()
{
}
