#ifndef UF_SFMLUp_GUI_Button_H
#define UF_SFMLUp_GUI_Button_H

#include <SFML/Graphics/Color.hpp>

//UseFull SFML Up Gui Button module
//Version 1.0 alfa
//Make by Graph Don'te-Crypto

#include "BaseGui.hpp"
#include "Frame.hpp"
#include "../Mouse.hpp"
#include "Label.hpp"

namespace sfup { namespace gui {

using namespace math;

struct Button : public BaseGui {
	Frame frame = Frame();
	sf::Color color_current;
	sf::Color color_focus_not;//(127, 127, 127, 255)
	sf::Color color_focus;    //(255, 255, 255, 255)
	sf::Color color_press;    //(063, 063, 063, 255)

	Label label;

	std::function<void (void)> lambda = nullptr;

	Button(const Codir<2> & codir) : BaseGui(codir), label(codir) {
		color_background = sf::Color::Black;
		label.color_outline = sf::Color::Transparent;
		frame = Frame(1, codir.center());
		
		/*
		color_focus_not = sf::Color(159, 159, 159, 255);
		color_focus 	= sf::Color(223, 223, 223, 255);
		color_press 	= sf::Color(191, 191, 191, 255);
		*/

		color_focus_not = sf::Color(127, 127, 127, 255);
		color_focus 	= sf::Color(255, 255, 255, 255);
		color_press 	= sf::Color(063, 063, 063, 255);
		
		setColor(color_focus_not);
	}

	void action() override {
		if (active) {
			FocusTracker::focus_offset = getAbsoluteOffset();
			checkFocus(Mouse.inWorld);
		}
	}
	~Button() {

	}
	void drawSelf() override {
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
		label.drawSelf();
		label.drawTo(*this);
	}
	void setColor(const sf::Color & color) {
		color_current = color;
		label.setTextColor(color_current);
		color_outline = color_current;
	}
	void actionNotFocused() override {
		setColor(color_focus_not);
	}
	void actionFocused() override {
		setColor(color_focus);
	}
	void actionPressed() override {
		setColor(color_press);
	}
	void actionRealized() override {
		setColor(color_focus);
		if (lambda != nullptr) lambda();
	}
	void moveRelative(const XY & delta) override {
		BaseGui::moveRelative(delta);
		//label.moveRelative(delta);
		frame.center  += delta;
	}
};

}}

#endif
