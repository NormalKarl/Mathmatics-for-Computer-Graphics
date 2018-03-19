#include "BitmapFont.h"

#include <fstream>
#include <sstream>
#include <string>

enum FileFocus {
	INFO,
	COMMON,
	CHARS,
	CHAR
};

BitmapFont::BitmapFont(const char* file) {
	std::ifstream i = std::ifstream(file, std::ios::in);

	if (i.is_open()) {
		FileFocus focus;
		std::string current;

		BitmapChar c;

		while (i >> current) {
			if (current == "info") {
				focus = INFO;
			}
			else if (current == "common") {
				focus = COMMON;
			}
			else if (current == "chars") {
				focus = CHARS;
			}
			else if (current == "char") {
				focus = CHAR;
			}
			else {
				switch (focus) {
				case CHAR:
					if(current.substr(0, 1) == "x")

					break;
				}
			}

			continue;
		}


	}
	else {
		printf("Error: Cannot open file %s.", file);
	}
}


BitmapFont::~BitmapFont()
{
}
