#ifndef UF_SFMLUP_Event_H
#define UF_SFMLUP_Event_H

#include <SFML/Graphics.hpp>

//UseFull SFML Up global Event module
//Version 1.1 alfa
//Make by Graph Don'te-Crypto

namespace sfup {
	enum class EventType {
		MouseMoved,

		MouseButtonPressed,
		MouseButtonPressedLeft,
		MouseButtonPressedRight,
		MouseButtonPressedMiddle,

		MouseButtonPressingStart,

		MouseButtonPressing,
		MouseButtonPressingLeft,
		MouseButtonPressingRight,
		MouseButtonPressingMiddle,

		MouseButtonPressingEnd,

		MouseButtonReleased,
		MouseButtonReleasedLeft,
		MouseButtonReleasedRight,
		MouseButtonReleasedMiddle,

		MouseWheelMoved,
		MouseWheelMovedUp,
		MouseWheelMovedDown,
	};
	
	enum class KeyState {Pressed, Pressing, Released};

	class EventKeeper {
	public:
		static constexpr size_t limit = 128;

		struct KeyPressedClass {
			bool e[limit];
			bool & operator [](sf::Keyboard::Key key) { return e[key];}
			bool & operator [](size_t key) { return e[key];}
		} KeyPressed;

		struct KeyPressingClass {
			bool e[limit];
			bool & operator [](sf::Keyboard::Key key) { return e[key];}
			bool & operator [](size_t key) { return e[key];}
		} KeyPressing;

		struct KeyReleasedClass {
			bool e[limit];
			bool & operator [](sf::Keyboard::Key key) { return e[key];}
			bool & operator [](size_t key) { return e[key];}
		} KeyReleased;

		struct KeyClass {
			KeyPressedClass  & KeyPressedRef;
			KeyPressingClass & KeyPressingRef;
			KeyReleasedClass & KeyReleasedRef;
			KeyClass(KeyPressedClass & p1, KeyPressingClass & p2, KeyReleasedClass & p3)
			: KeyPressedRef(p1)
			, KeyPressingRef(p2)
			, KeyReleasedRef(p3)
			{}
			bool & operator ()(sf::Keyboard::Key key, KeyState state) {
				switch (state) {
					case KeyState::Pressed : return KeyPressedRef [key];
					case KeyState::Pressing: return KeyPressingRef[key];
					case KeyState::Released: return KeyReleasedRef[key];
				}
				printf("Event.KeyClass.operator().FatalSwitchError\n");
				exit(1);
			}
			bool & operator ()(size_t key, KeyState state) {
				switch (state) {
					case KeyState::Pressed : return KeyPressedRef [key];
					case KeyState::Pressing: return KeyPressingRef[key];
					case KeyState::Released: return KeyReleasedRef[key];
				}
				printf("Event.KeyClass.operator().FatalSwitchError\n");
				exit(1);
			}
		} Key;

		bool e[limit];

		EventKeeper() : Key(KeyPressed, KeyPressing, KeyReleased) {
			flushStrong();
		}
		
		bool & operator [](sf::Keyboard::Key key)       { return e[key];}
		bool   operator [](sf::Keyboard::Key key) const { return e[key];}
		
		bool & operator [](size_t key)       { return e[key];}
		bool   operator [](size_t key) const { return e[key];}

		bool & operator [](EventType event)       { return e[(size_t)event];}
		bool   operator [](EventType event) const { return e[(size_t)event];}

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
				if (i <= (size_t)EventType::MouseButtonPressingStart || (size_t)EventType::MouseButtonPressingEnd <= i)
					e[i] = false;
				KeyReleased[i] = false;
				KeyPressed[i] = false;
			}
		}

		bool & getEvent(EventType event) {
			return e[(size_t)event];
		}

		void add(const sf::Event & event) {
			switch (event.type) {
			case sf::Event::MouseMoved:
				getEvent(EventType::MouseMoved) = true;
				getEvent(EventType::MouseMoved) = true;
				break;
			case sf::Event::MouseButtonPressed:
				getEvent(EventType::MouseButtonPressed)  = true;
				getEvent(EventType::MouseButtonPressing) = true;
				if (event.mouseButton.button == sf::Mouse::Left) {
					getEvent(EventType::MouseButtonPressedLeft)  = true;
					getEvent(EventType::MouseButtonPressingLeft) = true;
				}
				else if (event.mouseButton.button == sf::Mouse::Right) {
					getEvent(EventType::MouseButtonPressedRight)  = true;
					getEvent(EventType::MouseButtonPressingRight) = true;
				}
				else if (event.mouseButton.button == sf::Mouse::Middle) {
					getEvent(EventType::MouseButtonPressedMiddle)  = true;
					getEvent(EventType::MouseButtonPressingMiddle) = true;
				}
				break;

			case sf::Event::MouseButtonReleased:
				getEvent(EventType::MouseButtonReleased) = true;
				getEvent(EventType::MouseButtonPressing) = false;
				if (event.mouseButton.button == sf::Mouse::Left) {
					getEvent(EventType::MouseButtonReleasedLeft) = true;
					getEvent(EventType::MouseButtonPressingLeft) = false;
				}
				else if (event.mouseButton.button == sf::Mouse::Right) {
					getEvent(EventType::MouseButtonReleasedRight) = true;
					getEvent(EventType::MouseButtonPressingRight) = false;
				}
				else if (event.mouseButton.button == sf::Mouse::Middle) {
					getEvent(EventType::MouseButtonReleasedMiddle) = true;
					getEvent(EventType::MouseButtonPressingMiddle) = false;
				}
				break;
			case sf::Event::MouseWheelMoved:
				getEvent(EventType::MouseWheelMoved) = true;
				if (event.mouseWheel.delta == 1) 
					getEvent(EventType::MouseWheelMovedUp) = true;
				else if (event.mouseWheel.delta == -1) 
					getEvent(EventType::MouseWheelMovedDown) = true;
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
