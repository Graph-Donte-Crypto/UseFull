#ifndef UF_SFMLUp_GUI_Button_H
#define UF_SFMLUp_GUI_Button_H

#include "BaseGui.hpp"
#include "Frame.hpp"
#include "../Mouse.hpp"

//UseFull SFML Up Gui Button module
//Version 1.0 alfa
//Make by Graph Don'te-Crypto

namespace sfup { namespace gui {

using namespace math;

struct Button : public BaseGui {
	Frame frame = Frame();
	sf::Color color_current;
	sf::Color color_focus_not;//(127, 127, 127, 255)
	sf::Color color_focus;    //(255, 255, 255, 255)
	sf::Color color_press;    //(063, 063, 063, 255)

	sf::Text text;
	XY text_position = XY{0, 0};

	std::function<void (void)> lambda = nullptr;

	Button(const Codir<2> & codir) : BaseGui(codir) {
		frame = Frame(1, codir.center());
		color_focus_not = sf::Color(159, 159, 159, 255);
		color_focus 	= sf::Color(223, 223, 223, 255);
		color_press 	= sf::Color(191, 191, 191, 255);

		Fonts.setFontToText("UbuntuMono-R", text);

		text.setCharacterSize(14);
		text.setString("");
		text_position = codir.center();
		text.setFillColor(sf::Color::Black);
		text.setOutlineThickness(0.1);
		setColor(color_focus_not);
	}

	void centerTheText() {
		text_position[0] -= text.getGlobalBounds().width / 2;
		text_position[1] -= codir.size()[1] / 2;
	}

	void action() {
		if (active) {
			FocusTracker::focus_offset = getAbsoluteOffset();
			checkFocus(Mouse.inWorld);
		}
	}
	~Button() {

	}
	void drawSelf() {
		BaseGui::drawSelf();

		frame.color = color_current;
		frame.drawTo(*this);

		/*
		text.setPosition(text_position[0], text_position[1]);
		{
			sf::Font temp_font;
			sf::Text text("", *Fonts.getByName("UbuntuMono-R").valueOr(&temp_font), 14);
			text.setPosition(0, 0);
			text.setString("Test");
			text.setColor(sf::Color::Black);
			this->draw(text);
		}
		*/

		this->draw(text);
	}
	void setColor(const sf::Color & color) {
		color_current = color;
		color_background = color_current;
	}
	void actionNotFocused() {
		setColor(color_focus_not);
	}
	void actionFocused() {
		setColor(color_focus);
	}
	void actionPressed() {
		setColor(color_press);
	}
	void actionRealized() {
		setColor(color_focus);
		if (lambda != nullptr) lambda();
	}
	void moveRelative(const XY & delta) {
		BaseGui::moveRelative(delta);
		frame.center  += delta;
		text_position += delta;
	}
};

}}

#endif
