#ifndef UF_SFMLUP_Event_H
#define UF_SFMLUP_Event_H

#include <SFML/Graphics.hpp>

//UseFull SFML Up global Event module
//Version 1.1 alfa
//Make by Graph Don'te-Crypto

namespace sfup {
	enum ev {
		evMouseMoved,

		evMouseButtonPressed,
		evMouseButtonPressedLeft,
		evMouseButtonPressedRight,
		evMouseButtonPressedMiddle,

		evMouseButtonPressingStart,

		evMouseButtonPressing,
		evMouseButtonPressingLeft,
		evMouseButtonPressingRight,
		evMouseButtonPressingMiddle,

		evMouseButtonPressingEnd,

		evMouseButtonReleased,
		evMouseButtonReleasedLeft,
		evMouseButtonReleasedRight,
		evMouseButtonReleasedMiddle,

		evMouseWheelMoved,
		evMouseWheelMovedUp,
		evMouseWheelMovedDown,
	};
	
	enum class KeyState {Pressed, Pressing, Released};

	class EventKeeper {
	public:
		static constexpr size_t limit = 128;

		class KeyPressedClass {
		public:
			bool e[limit];
			bool & operator [](sf::Keyboard::Key key) { return e[key];}
			bool & operator [](size_t key) { return e[key];}
		} KeyPressed;

		class KeyPressingClass {
		public:
			bool e[limit];
			bool & operator [](sf::Keyboard::Key key) { return e[key];}
			bool & operator [](size_t key) { return e[key];}
		} KeyPressing;

		class KeyReleasedClass {
		public:
			bool e[limit];
			bool & operator [](sf::Keyboard::Key key) { return e[key];}
			bool & operator [](size_t key) { return e[key];}
		} KeyReleased;

		class KeyClass {
		public:
			KeyPressedClass  * KeyPressedPtr  = nullptr;
			KeyPressingClass * KeyPressingPtr = nullptr;
			KeyReleasedClass * KeyReleasedPtr = nullptr;
			KeyClass() {
			}
			bool & operator ()(sf::Keyboard::Key key, KeyState state) {
				switch (state) {
					case KeyState::Pressed:  return (*KeyPressedPtr) [key];
					case KeyState::Pressing: return (*KeyPressingPtr)[key];
					case KeyState::Released: return (*KeyReleasedPtr)[key];
				}
				printf("Event.KeyClass.operator().FatalSwitchError\n");
				exit(1);
			}
			bool & operator ()(size_t key, KeyState state) {
				switch (state) {
					case KeyState::Pressed:  return (*KeyPressedPtr) [key];
					case KeyState::Pressing: return (*KeyPressingPtr)[key];
					case KeyState::Released: return (*KeyReleasedPtr)[key];
				}
				printf("Event.KeyClass.operator().FatalSwitchError\n");
				exit(1);
			}
		} Key;

		bool e[limit];

		EventKeeper() {
			flushStrong();
			Key.KeyPressedPtr  = &KeyPressed;
			Key.KeyPressingPtr = &KeyPressing;
			Key.KeyReleasedPtr = &KeyReleased;
		}
		
		bool & operator [](sf::Keyboard::Key key)       { return e[key];}
		bool   operator [](sf::Keyboard::Key key) const { return e[key];}
		
		bool & operator [](size_t key)       { return e[key];}
		bool   operator [](size_t key) const { return e[key];}

		void flushStrong() {
			for (size_t i = 0; i < limit; i++) {
				e[i] = false;
				KeyPressing[i] = false;
				KeyPressed [i] = false;
				KeyReleased[i] = false;
			}
		}
		void flush() {
			for (size_t i = 0; i < limit; i++) {
				if (i <= evMouseButtonPressingStart || evMouseButtonPressingEnd <= i) e[i] = false;
				KeyReleased[i] = false;
				KeyPressed[i] = false;
			}
		}
		void add(const sf::Event & event) {
			switch (event.type) {
			case sf::Event::MouseMoved:
				e[evMouseMoved] = true;
				break;
			case sf::Event::MouseButtonPressed:
				e[evMouseButtonPressed]  = true;
				e[evMouseButtonPressing] = true;
				if (event.mouseButton.button == sf::Mouse::Left) {
					e[evMouseButtonPressedLeft]  = true;
					e[evMouseButtonPressingLeft] = true;
				}
				else if (event.mouseButton.button == sf::Mouse::Right) {
					e[evMouseButtonPressedRight]  = true;
					e[evMouseButtonPressingRight] = true;
				}
				else if (event.mouseButton.button == sf::Mouse::Middle) {
					e[evMouseButtonPressedMiddle]  = true;
					e[evMouseButtonPressingMiddle] = true;
				}
				break;

			case sf::Event::MouseButtonReleased:
				e[evMouseButtonReleased] = true;
				e[evMouseButtonPressing] = false;
				if (event.mouseButton.button == sf::Mouse::Left) {
					e[evMouseButtonReleasedLeft] = true;
					e[evMouseButtonPressingLeft] = false;
				}
				else if (event.mouseButton.button == sf::Mouse::Right) {
					e[evMouseButtonReleasedRight] = true;
					e[evMouseButtonPressingRight] = false;
				}
				else if (event.mouseButton.button == sf::Mouse::Middle) {
					e[evMouseButtonReleasedMiddle] = true;
					e[evMouseButtonPressingMiddle] = false;
				}
				break;
			case sf::Event::MouseWheelMoved:
				e[evMouseWheelMoved] = true;
				if (event.mouseWheel.delta == 1) 
					e[evMouseWheelMovedUp] = true;
				else if (event.mouseWheel.delta == -1) 
					e[evMouseWheelMovedDown] = true;
				break;
			case sf::Event::KeyPressed:
				KeyPressed[event.key.code]  = true;
				KeyPressing[event.key.code] = true;
				break;

			case sf::Event::KeyReleased:
				KeyReleased[event.key.code] = true;
				KeyPressing[event.key.code] = false;
				break;
			default:
				break;
			}
		}
		void load(sf::RenderWindow & window) {
			sf::Event event;
			while (window.pollEvent(event)) {
				switch (event.type) {
				case sf::Event::Closed:
					window.close();
					break;
				default:
					add(event);
					break;
				}
			}
		}
	}
	Event;
}

#endif