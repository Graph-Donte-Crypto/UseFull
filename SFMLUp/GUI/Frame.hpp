#ifndef UF_SFMLUp_GUI_Frame_H
#define UF_SFMLUp_GUI_Frame_H

#include "../../Utils/StdDiagnosticIgnore.hpp"

#include <SFML/Graphics.hpp>

#include "../../Utils/StdDiagnosticIgnoreEnd.hpp"

//UseFull SFML Up Gui Frame module
//Version 1.0 alfa
//Make by Graph Don'te-Crypto

#include "../../Templates/Array.hpp"
#include "../../Math/Vector.hpp"
#include "../Drawer.hpp"

namespace sfup { namespace gui {		
	using namespace uft;

	struct Frame {
		Array<XY> vecs = Array<XY>(8);
		Array<size_t> breaks = Array<size_t>(1);
		XY center = XY {0, 0};
		sf::Color color;
		
		Frame() {
			
		}
		
		Frame & set(const Frame & frame) {
			vecs.flushAndCopyFrom(frame.vecs);
			breaks.flushAndCopyFrom(frame.breaks);
			center = frame.center;
			color = frame.color;
			return *this;
		}
		
		Frame(const Frame & frame) {
			set(frame);
		}
		Frame & operator = (const Frame & frame) {
			return set(frame);
		}
		
		Frame(size_t new_limit, XY new_center = XY{0, 0}) {
			vecs.resizeMemoryWithFlush(new_limit);
			center = new_center;
		}
		~Frame() {
		}
		//relative:
		//  true  -> разместить вектор относительно center
		//  false -> разместить вектор относительно XY(0,0)
		Frame & addPoint(const XY & vec, bool relative = true) {
			vecs.addCopy(relative ? vec - center : vec);
			return *this;
		}
		Frame & addBreak() {
			breaks.addCopy(vecs.length - 1);
			return *this;
		}
		void drawTo(sf::RenderTarget & target) {
			if (vecs.length > 0) {
				if (breaks.length > 0) {
					for (size_t j = 0; j < breaks[0]; j++) 
						Drawer.drawLine(target, Line(vecs[j], vecs[j + 1]) + center, color);
					
					for (size_t i = 0; i < breaks.length - 1; i++) {
						for (size_t j = breaks[i] + 1; j < breaks[i + 1]; j++)
							Drawer.drawLine(target, Line(vecs[j], vecs[j + 1]) + center, color);
					}
					
					for (size_t j = breaks.last() + 1; j < vecs.length; j++) 
						Drawer.drawLine(target, Line(vecs[j], vecs[j + 1]) + center, color);
					if (breaks.last() != vecs.length - 1)
						Drawer.drawLine(target, Line(vecs.last(), vecs[0]) + center, color);
				}
				else {
					for (size_t j = 0; j < vecs.length; j++)
						Drawer.drawLine(target, Line(vecs[j], vecs[j + 1]) + center, color);
					Drawer.drawLine(target, Line(vecs.last(), vecs[0]) + center, color);
				}
			}
		}
		//Относительно центра
		void scale(const double & scale_coef) {
			vecs.foreach([&scale_coef](XY & xy){xy *= scale_coef;});
		}
		//Относительно центра
		//Против часовой стрелки?
		void rotate(const double & angle) {
			vecs.foreach([&angle](XY & xy){
				xy = {
					xy[0] * cos(angle) - xy[1] * sin(angle),
					xy[0] * sin(angle) + xy[1] * cos(angle)
				};
			});
		}
		//Собирает сумму всех векторов относительно текущего центра
		//И изменяет текущий центр так, чтобы он находился в средней точке всех векторов
		void makeRelativeCenter() {
			XY new_center = {0, 0};
			vecs.foreach([&new_center](XY & xy){new_center += xy;});
			new_center /= vecs.length;

			center += new_center;
			vecs.foreach([&new_center](XY & xy){xy -= new_center;});
		}
		//Перемещает центр не изменяя положение векторов относительно экрана
		void makeSpecialCenter(const XY & new_center) {
			XY delta = center - new_center;
			vecs.foreach([&delta](XY & xy){xy += delta;});
			center = new_center;
		}
	};

}}

#endif