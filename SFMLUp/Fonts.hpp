#ifndef UF_SFMLUp_Fonts_H
#define UF_SFMLUp_Fonts_H

#include <SFML/Graphics.hpp>
#include "../Templates/Array.hpp"
#include "../Utils/Ok.hpp"

#include <vector>

//UseFull SFML Up Fonts storage module
//Version 1.0 alfa
//Make by Graph Don'te-Crypto

namespace sfup {

	using namespace utils;

	struct FontsStruct {
		//magic std::vector fix stupid sf::Font in dynamic memory
		std::vector<sf::Font>    fonts;
		//
		uft::Array<const char *> names;
		
		FontsStruct() {
			
		}
		
		void load(const char * path, const char * name) {
			sf::Font font;
			if (font.loadFromFile(path)){
				fonts.push_back(font);
				names.addCopy(name);
			}
			printf("Font [%s] loaded\n", name);
		}
		
		Ok<sf::Font *> getByName(const char * name) {
			Ok<size_t> index = names.indexByCondition([&name](const char ** n) -> bool {
				return strcmp(name, *n) == 0;
			});
			if (index.isOk) return &(fonts[index]);
			else return {};
		}
		
		void setFontToText(const char * name, sf::Text & text) {
			Ok<sf::Font *> font = getByName(name);
			if (font.isOk) text.setFont(*(font.value));
			else {
				printf("Error find font [%name]\n", name);
				font = getDefault();
				if (font.isOk) text.setFont(*(font.value));
				else printf("Error find default font\n");
			}
		}
		
		Ok<sf::Font *> getDefault() {
			if (fonts.size() > 0) return &(fonts[0]);
			else return {};
		}
	} 
	Fonts;

}

#endif