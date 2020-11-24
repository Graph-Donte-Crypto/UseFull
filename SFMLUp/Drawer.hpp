#ifndef UF_SFMLUP_Drawer_H
#define UF_SFMLUP_Drawer_H

#include <SFML/Graphics.hpp>
#include "../Math/Shape.hpp"
#include "Fonts.hpp"

#include <sstream>

//UseFull SFML Up Drawer module
//Version 1.0 alfa
//Make by Graph Don'te-Crypto

namespace sfup {
	using namespace math;
	using namespace utils;
	
	struct DrawerStruct {
		
		sf::RenderTarget * target = nullptr;
		
		sf::Text text;
		std::ostringstream text_stream;
		std::ostringstream & Text = text_stream;
			
		DrawerStruct() {
			text.setString("");
			text.setCharacterSize(14);
		}
		
		Vector<2> getTextSize() {
			sf::FloatRect fr = text.getGlobalBounds();
			return Vector<2>({fr.width, fr.height});
		}
		Vector<2> getTextSize(const sf::Text & text) {
			sf::FloatRect fr = text.getGlobalBounds();
			return Vector<2>({fr.width, fr.height});
		}

		template <typename T>
		void drawText(T some, const Vector<2> & xy) {
			text_stream << some;
			text.setPosition(xy[0], xy[1]);
			text.setString(text_stream.str());
			target->draw(text);
			text_stream.str("");
		}
		void drawText(const Vector<2> & xy) {
			text.setPosition(xy[0], xy[1]);
			text.setString(text_stream.str());
			target->draw(text);
			text_stream.str("");
		}
		
		void drawCircle(sf::RenderTarget * rt, const Sphere<2> & circle, const sf::Color & color) {
			sf::CircleShape circle_shape;
			circle_shape.setRadius(circle.r);
			circle_shape.setPosition(circle.center[0] - circle.r, circle.center[1] - circle.r);
			circle_shape.setFillColor(color);
			rt->draw(circle_shape);
		}

		void drawLine(sf::RenderTarget * rt, const Line<2> & line, const sf::Color & color) {
			sf::Vertex vertexes[2] = {
				sf::Vertex(sf::Vector2f(line.a[0], line.a[1]), color),
				sf::Vertex(sf::Vector2f(line.b[0], line.b[1]), color)
			};
			rt->draw(vertexes, 2, sf::Lines);
		}
			
		void drawCodir(sf::RenderTarget * rt, const Codir<2> & c, const sf::Color & outline) {
			sf::RectangleShape shape(
				sf::Vector2f(
					c.right_down[0] - c.left_up[0],
					c.right_down[1] - c.left_up[1]
				)
			);
			shape.setPosition(sf::Vector2f(c.left_up[0], c.left_up[1]));
			shape.setOutlineColor(outline);
			shape.setFillColor(sf::Color(0, 0, 0, 0));
			shape.setOutlineThickness(1);
			rt->draw(shape);
		}
		
		void drawCodirFilled(sf::RenderTarget * rt, const Codir<2> & c, const sf::Color & outline, const sf::Color & background) {
			sf::RectangleShape shape(
				sf::Vector2f(
					c.right_down[0] - c.left_up[0],
					c.right_down[1] - c.left_up[1]
				)
			);
			shape.setPosition(sf::Vector2f(c.left_up[0], c.left_up[1]));
			shape.setFillColor(background);
			shape.setOutlineColor(outline);
			shape.setOutlineThickness(1);
			rt->draw(shape);
		}
		
		void drawCircle(const Sphere<2> & circle, const sf::Color & color) {
			drawCircle(target, circle, color);
		}

		void drawLine(const Line<2> & line, const sf::Color & color) {
			drawLine(target, line, color);
		}
			
		void drawCodir(const Codir<2> & c, const sf::Color & color) {
			drawCodir(target, c, color);
		}
		
		void drawCodirFilled(const Codir<2> & c, const sf::Color & outline, const sf::Color & background) {
			drawCodirFilled(target, c, outline, background);
		}
	} Drawer;
}

#endif