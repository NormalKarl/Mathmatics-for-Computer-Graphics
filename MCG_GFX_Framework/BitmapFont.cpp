#include "BitmapFont.h"

#include <fstream>
#include <sstream>
#include <string>

#include "tinyxml2.h"
#include "Rasterizer.h"

BitmapFont::BitmapFont(const char* filename, std::vector<const char*> pagePaths) {
	//Fill chars array with empty BitmapChar objects.
	std::fill_n((char*)chars, sizeof(BitmapChar) * 256, NULL);

	tinyxml2::XMLDocument doc;
	doc.LoadFile(filename);

	tinyxml2::XMLElement* commonEle = doc.FirstChildElement("font")->FirstChildElement("common");
	lineHeight = commonEle->IntAttribute("lineHeight");
	base = commonEle->IntAttribute("base");
	scaleW = commonEle->IntAttribute("scaleW");
	scaleH = commonEle->IntAttribute("scaleH");
	pages = commonEle->IntAttribute("pages");

	tinyxml2::XMLElement* e = doc.FirstChildElement("font")->FirstChildElement("chars");
	texture = Texture(pagePaths[0]);

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

		//Transform the x, y, width and height of the sprite into normalized positions.
		//This is used for the Rasterizer class.
		ch.uvX = (float)ch.x / texture.getWidthF();
		ch.uvY = (float)ch.y / texture.getHeightF();
		ch.uvWidth = (float)ch.width / texture.getWidthF();
		ch.uvHeight = (float)ch.height / texture.getHeightF();

		chars[ch.id] = ch;
	}


	tinyxml2::XMLElement* kerningsElement = doc.FirstChildElement("font")->FirstChildElement("kernings");

	for (tinyxml2::XMLElement* c = (tinyxml2::XMLElement*)kerningsElement->FirstChild(); c != NULL; c = (tinyxml2::XMLElement*)c->NextSibling()) {
		Kerning k = { 0 };

		k.first = c->IntAttribute("first");
		k.second = c->IntAttribute("second");
		k.amount = c->IntAttribute("amount");

		kerning[k.second].push_back(k);
	}

	return;
}


BitmapFont::~BitmapFont()
{
}

void BitmapFont::drawText(Rasterizer* rasterizer, std::string text, float x, float y) {
	float currentX = x;

	rasterizer->bindTexture(&texture);
	
	for (int c = 0; c < text.size(); c++) {
		BitmapChar bc = chars[text[c]];

		float charX = currentX + bc.xoffset;
		float charY = (y - lineHeight + base) + bc.yoffset;

		if (kerning[c].size() != 0 && c != 0) {
			for (int k = 0; k < kerning[c].size(); k++) {
				if (kerning[c][k].first == text[c - 1]) {
					charX += kerning[c][k].amount;
				}
			}
		}

		Vertex vertices[] = { { charX, charY, 0, bc.uvX, bc.uvY },
							 { charX + bc.width, charY, 0, bc.uvX + bc.uvWidth, bc.uvY },
							 { charX + bc.width, charY + bc.height, 0, bc.uvX + bc.uvWidth, bc.uvY + bc.uvHeight },
							 { charX, charY + bc.height, 0, bc.uvX, bc.uvY + bc.uvHeight } };

		currentX += bc.xadvance;

		rasterizer->drawQuad(vertices[0], vertices[1], vertices[2], vertices[3]);
	}
}