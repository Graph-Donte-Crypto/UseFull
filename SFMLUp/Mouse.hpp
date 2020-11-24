#ifndef UF_SFMLUP_Mouse_H
#define UF_SFMLUP_Mouse_H

#include <SFML/Graphics.hpp>
#include "../Math/Vector.hpp"

//UseFull SFML Up Mouse module
//Version 1.0 alfa
//Make by Graph Don'te-Crypto

namespace sfup {
	using namespace math;
	
	struct MouseStruct {
		
		sf::RenderWindow * window = nullptr;
		
		XY onDesktop, inWindow, inWorld;
		
		void getPosition() {
			onDesktop = getDesktopPosition();
			inWindow = getWindowPosition(*window);
			inWorld = getWorldPosition(*window);
		}
		
		void getPosition(sf::RenderWindow & window) {
			onDesktop = getDesktopPosition();
			inWindow = getWindowPosition(window);
			inWorld = getWorldPosition(window);
		}
		
		XY getDesktopPosition() {
			auto pos = sf::Mouse::getPosition();
			return {pos.x, pos.y}; 
		}
		
		XY getWindowPosition(sf::RenderWindow & window) {
			sf::Vector2i pos = sf::Mouse::getPosition(window);
			return {pos.x, pos.y};
		}
		
		XY getWindowPosition() {
			return getWindowPosition(*window);
		}
		
		XY getWorldPosition(sf::RenderWindow & window) {
			auto pos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
			return {pos.x, pos.y};
		}
		
		XY getWorldPosition() {
			return getWorldPosition(*window);
		}		
	} Mouse;
}

#endif