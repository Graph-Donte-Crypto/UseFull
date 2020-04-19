#ifndef UF_M_Equation_H
#define UF_M_Equation_H

#include "Vector.h"
#include "Shape.h"

#include <initializer_list>

//UseFull Math Equation module
//Version 1.0 alfa
//Make by Graph Don'te-Crypto

namespace math {
	template <size_t dimension>
	struct EquationLine {
		/*
		coords = point + t * vector;
		*/
		
		Vector<dimension> point;
		Vector<dimension> vector;
		
		//
		
		EquationLine(const Vector<dimension> * a, const Vector<dimension> * b) {
			point = *a;
			vector = (*b - *a).ort();
		}
		
		EquationLine(const Line<dimension> * line) {
			point = line->a;
			vector = (line->b - line->a).ort();
		}
		
		EquationLine<dimension> & printf() {
			::printf("point = "); point.printf();
			::printf("vector = "); vector.printf();
			return *this;
		}
		
	};
	
	template <size_t dimension>
	struct EquationHyperplane {
		/*
		a1 * x1 + ... an * xn + c = 0	
		*/
		Vector<dimension> a;
		double c;
		EquationHyperplane() {
			
		}
		EquationHyperplane(const Vector<dimension> & vector, double xc) {
			a = vector;
			c = xc;
		}
		EquationHyperplane(const EquationHyperplane<dimension> & e) {
			a = e.a;
			c = e.c;
		}
		
		EquationHyperplane(const Vector<dimension> & point, const Vector<dimension> & ort) {
			a = ort;
			c = - (point * ort);
		}
		
		EquationHyperplane<dimension> & printf() {
			::printf("a = "); a.printf();
			::printf("c = %lf\n", c); 
			return *this;
		}
	};
	
	struct EquationLine2D {
		/*
		A * x + B * y + C = 0	
		*/
		double a = 0;
		double b = 0;
		double c = 0;
		
		EquationLine(double xa, double xb, double xc) {
			a = xa;
			b = xb;
			c = xc;
		}
		
		EquationLine(const Line2D & line) {
			/*
				(y - y1) / (y2 - y1) = (x - x1) / (x2 - x1)
				
				(y1 - y2) * x + (x2 - x1) * y + (x1 * y2 - x2 * y1) = 0
					A				B					 C
			*/
			a = line.a[1] - line.b[1];
			b = line.b[0] - line.a[0];
			c = line.a[0] * line.b[1] - line.b[0] * line.a[1];
		}
		
	};
}

#endif