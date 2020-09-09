#ifndef UF_SFMLUP_MousePos_H
#define UF_SFMLUP_MousePos_H

#include <SFML/Graphics.hpp>

#include "../Math/Vector.hpp"

//UseFull SFML Up Mouse Position in Vector module
//Version 1.0 release
//Make by Graph Don'te-Crypto

namespace sfup {
	math::XY getMousePos(sf::RenderWindow & window) {
		sf::Vector2i pos = sf::Mouse::getPosition(window);
		return {pos.x, pos.y};
	}
}

#endif