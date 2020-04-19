#ifndef UF_SFMLUP_MousePos_H
#define UF_SFMLUP_MousePos_H

#include <SFML/Graphics.hpp>

#include "../Math/xy.h"

//UseFull SFML Up Mouse Position in XY module
//Version 1.0 alfa
//Make by Graph Don'te-Crypto

namespace sfup {
	math::XY getMousePos(sf::RenderWindow & window) {
		sf::Vector2i pos = sf::Mouse::getPosition(window);
		return math::XY(pos.x, pos.y);
	}
}



#endif