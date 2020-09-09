#ifndef UF_SFMLUP_Drawer_H
#define UF_SFMLUP_Drawer_H

#include <SFML/Graphics.hpp>
#include "../Math/Shape.hpp"

#include <sstream>

//UseFull SFML Up Drawer module
//Version 1.0 alfa
//Make by Graph Don'te-Crypto

namespace sfup {
	using namespace math;
	
	struct DrawerStruct {
		
		sf::RenderWindow * window = nullptr;
		
		sf::Text text;
		sf::Font ubuntu_mono_r;
		std::ostringstream text_stream;
		std::ostringstream & Text = text_stream;
			
		DrawerStruct() {
			ubuntu_mono_r.loadFromFile("ubuntumono/UbuntuMono-R.ttf");
			text.setString("");
			text.setFont(ubuntu_mono_r);
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
			window->draw(text);
			text_stream.str("");
		}
		void drawText(const Vector<2> & xy) {
			text.setPosition(xy[0], xy[1]);
			text.setString(text_stream.str());
			window->draw(text);
			text_stream.str("");
		}
		
		void drawCircle(sf::RenderWindow & rw, const Sphere<2> & circle, const sf::Color & color) {
			sf::CircleShape circle_shape;
			circle_shape.setRadius(circle.r);
			circle_shape.setPosition(circle.center[0] - circle.r, circle.center[1] - circle.r);
			circle_shape.setFillColor(color);
			rw.draw(circle_shape);
		}

		void drawLine(sf::RenderWindow & rw, const Line<2> & line, const sf::Color & color) {
			sf::Vertex vertexes[2] = {
				sf::Vertex(sf::Vector2f(line.a[0], line.a[1]), color),
				sf::Vertex(sf::Vector2f(line.b[0], line.b[1]), color)
			};
			rw.draw(vertexes, 2, sf::Lines);
		}
			
		void drawCodir(sf::RenderWindow & rw, const Codir<2> & c, const sf::Color & outline) {
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
			rw.draw(shape);
		}
		
		void drawCodirFilled(sf::RenderWindow & rw, const Codir<2> & c, const sf::Color & outline, const sf::Color & background) {
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
			rw.draw(shape);
		}
		
		void drawCircle(const Sphere<2> & circle, const sf::Color & color) {
			drawCircle(*window, circle, color);
		}

		void drawLine(const Line<2> & line, const sf::Color & color) {
			drawLine(*window, line, color);
		}
			
		void drawCodir(const Codir<2> & c, const sf::Color & color) {
			drawCodir(*window, c, color);
		}
		
		void drawCodirFilled(const Codir<2> & c, const sf::Color & outline, const sf::Color & background) {
			drawCodirFilled(*window, c, outline, background);
		}
	} Drawer;
}

#endif