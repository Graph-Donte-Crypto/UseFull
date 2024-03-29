#ifndef UF_SFMLUP_Drawer_H
#define UF_SFMLUP_Drawer_H

#include "../Utils/StdDiagnosticIgnore.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <cstdlib>
#include <sstream>

#include "../Utils/StdDiagnosticIgnoreEnd.hpp"

//UseFull SFML Up Drawer module
//Version 1.0 alfa
//Make by Graph Don'te-Crypto

#include "../Math/Equation.hpp"
#include "Fonts.hpp"
#include "View.hpp"

namespace sfup {
	using namespace math;
	using namespace utils;
	struct DrawerStruct {

		sf::RenderTarget * target = nullptr;
		sf::View * last_view = nullptr;
		sf::Text text;
		std::ostringstream text_stream;
		std::ostringstream & Text = text_stream;

		DrawerStruct() {
			text.setString("");
			text.setCharacterSize(14);
		}

		XY getTextSize(const sf::Text & text) {
			sf::FloatRect fr = text.getGlobalBounds();
			return XY({fr.width, fr.height});
		}
		XY getTextSize() {
			return getTextSize(text);
		}

		template <typename T>
		void drawText(sf::RenderTarget & rt,  const T & some, const Vector<2> & xy) {
			text_stream << some;
			text.setPosition(xy[0], xy[1]);
			text.setString(text_stream.str());
			rt.draw(text);
			text_stream.str("");
		}
		void drawText(sf::RenderTarget & rt, const Vector<2> & xy) {
			text.setPosition(xy[0], xy[1]);
			text.setString(text_stream.str());
			rt.draw(text);
			text_stream.str("");
		}

		void drawCircle(sf::RenderTarget & rt, const Sphere<2> & circle, const sf::Color & color) {
			sf::CircleShape circle_shape;
			circle_shape.setRadius(circle.r);
			circle_shape.setPosition(circle.center[0] - circle.r, circle.center[1] - circle.r);
			circle_shape.setFillColor(color);
			rt.draw(circle_shape);
		}

		void drawLine(sf::RenderTarget & rt, const Line<2> & line, const sf::Color & color) {
			sf::Vertex vertexes[2] = {
				sf::Vertex(sf::Vector2f(line.a[0], line.a[1]), color),
				sf::Vertex(sf::Vector2f(line.b[0], line.b[1]), color)
			};
			rt.draw(vertexes, 2, sf::Lines);
		}

		void drawCodir(sf::RenderTarget & rt, const Codir<2> & c, const sf::Color & outline, const sf::Color & background) {
			sf::RectangleShape shape(
				sf::Vector2f(
					c.right_down[0] - c.left_up[0],
					c.right_down[1] - c.left_up[1]
				)
			);
			shape.setPosition(sf::Vector2f(c.left_up[0], c.left_up[1]));
			shape.setFillColor(background);
			shape.setOutlineColor(outline);
			shape.setOutlineThickness(-1);
			rt.draw(shape, sf::BlendNone);
		}

		//TODO: FIX
		void drawHyperplane(
			sf::RenderTarget & rt, 
			const EquationHyperplane<2> & eh, 
			const sf::Color & ort_color,
			const sf::Color & line_color
		) {
			XY start_point;
			if (abs(eh.ort[0]) > 0.001) {
				start_point[1] = 1;
				start_point[0] = ( (-eh.c) - eh.ort[1] * start_point[1]) / eh.ort[0];
			}
			else if (abs(eh.ort[1]) > 0.001) {
				start_point[0] = 1;
				start_point[1] = ( (-eh.c) - eh.ort[0] * start_point[0]) / eh.ort[1];
			}
			else {
				start_point = {0, 0};
			}
			XY p1 = ((eh.ort * (-eh.c)) - start_point).ort() * 30;
			drawLine(rt, Line(start_point + p1, start_point    ).lengthenBy(30, LengthenWay::TowardsB), line_color);
			drawLine(rt, Line(start_point, start_point + eh.ort).lengthenBy(20, LengthenWay::TowardsB), ort_color);
		}

		void drawCircle(const Sphere<2> & circle, const sf::Color & color) {
			drawCircle(*target, circle, color);
		}

		void drawLine(const Line<2> & line, const sf::Color & color) {
			drawLine(*target, line, color);
		}

		void drawCodir(const Codir<2> & c, const sf::Color & outline, const sf::Color & background) {
			drawCodir(*target, c, outline, background);
		}

		template <typename T>
		void drawText(const T & some, const Vector<2> & xy) {
			drawText(*target, some, xy);
		}
		void drawText(const Vector<2> & xy) {
			drawText(*target, xy);
		}

		void useCustomView(CoCustomView auto & cview) {
			if (last_view != &cview.view) {
				last_view = &cview.view;
				cview.use();
			}
		}
		void drawText(CoCustomView auto & cview, const Vector<2> & xy) {
			useCustomView(cview);
			drawText(xy);
		}
		template <typename T>
		void drawText(CoCustomView auto & cview, const T & some, const Vector<2> & xy) {
			useCustomView(cview);
			drawText(some, xy);
		}

		void drawCircle(CoCustomView auto & cview, const Sphere<2> & circle, const sf::Color & color) {
			useCustomView(cview);
			drawCircle(*target, circle, color);
		}

		void drawLine(CoCustomView auto & cview, const Line<2> & line, const sf::Color & color) {
			useCustomView(cview);
			drawLine(*target, line, color);
		}

		void drawCodir(CoCustomView auto & cview, const Codir<2> & c, const sf::Color & outline, const sf::Color & background) {
			useCustomView(cview);
			drawCodir(*target, c, outline, background);
		}

	} Drawer;
}

#endif
