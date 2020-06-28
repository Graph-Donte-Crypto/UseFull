#ifndef UF_SFMLUP_Console_H
#define UF_SFMLUP_Console_H

#include <SFML/Graphics.hpp>

//UseFull SFML Up Console module
//Version 1.0 alfa
//Make by Graph Don'te-Crypto

namespace sfmlup {
	struct OneCommandConsole {
		Codir<2> codir;
		uft::Array<char> string;
		bool selection = false;
		size_t position = 0;
		size_t selection_start, selection_end;
		
		OneCommandConsole(const Vector<2> & p1, const Vector<2> & p2) {
			codir = Codir<2>(p1, p2);
		}
		static OneCommandConsole fromPositionAndEndPosition(const Vector<2> & p1, const Vector<2> & p2) {
			return OneCommandConsole(p1, p2);
		}
		static OneCommandConsole fromPositionAndSize(const Vector<2> & p, const Vector<2> & s) {
			return OneCommandConsole(p, p + s);
		}
		
		void checkControlKeys() {
			if (Event.KeyPressed[sf::Keyboard::LShift] || Event.KeyPressed[sf::Keyboard::RShift]) {
				selection_start = position;
				selection = true;
			}
			if (Event.KeyReleased[sf::Keyboard::LShift] || Event.KeyReleased[sf::Keyboard::RShift]) {
				selection_start = position;
				selection = false;
			}
			if (selection) selection_end = position;
		}
		
		void addSymbol() {
			
		}
		
		void draw() {
			Global.drawCodir(codir, sf::Color::White);
		}
	};
}

#endif