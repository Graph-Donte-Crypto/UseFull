#ifndef UF_SFMLUp_GUI_Label_H
#define UF_SFMLUp_GUI_Label_H

#include "../../Utils/StdDiagnosticIgnore.hpp"

#include <SFML/Graphics/Color.hpp>

#include "../../Utils/StdDiagnosticIgnoreEnd.hpp"

//UseFull SFML Up Gui Label module
//Version 1.0 alfa
//Make by Graph Don'te-Crypto

#include "BaseGui.hpp"

namespace sfup { namespace gui {

using namespace math;

struct Label : public BaseGui {
	sf::Text text;

	Label(const Codir<2> & codir) : BaseGui(codir), text() {
		color_background = sf::Color::Transparent;
		color_outline = sf::Color::White;

		Fonts.setFontToText("UbuntuMono-R", text);

		text.setCharacterSize(14);
		text.setString("");
		text.setOutlineThickness(1);
		setTextColor(sf::Color::White);
		
		centerTheText();
	}

	void setText(const char * str) {
		text.setString(str);
		centerTheText();
	}

	template <typename Type> 
	void setText(const Type & object) {
		std::string temp_string = str::toStringFormat(object);
		text.setString(temp_string);
		centerTheText();
	}

	void centerTheText() {
		XY text_position = codir.center();
		text_position[0] -= text.getGlobalBounds().width / 2;
		text_position[1] -= codir.size()[1] / 2;
		text.setPosition(text_position[0], text_position[1]);
	}
	~Label() {

	}
	void drawSelf() {
		BaseGui::drawSelf();
		this->draw(text);
	}
	void setTextColor(const sf::Color & color) {
		text.setFillColor(color);
	}
	const sf::Color & getTextColor() const {
		return text.getFillColor();
	}
	void moveRelative(const XY & delta) {
		BaseGui::moveRelative(delta);
		sf::Vector2f vec = text.getPosition();
		text.setPosition(vec.x + delta[0], vec.y + delta[1]);
	}
};

}}

#endif
