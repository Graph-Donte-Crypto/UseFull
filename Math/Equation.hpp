#ifndef UF_M_Equation_H
#define UF_M_Equation_H

#include "Vector.hpp"
#include "Shape.hpp"

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
		EquationLine(const Vector<dimension> & a, const Vector<dimension> & b) {
			point = a;
			vector = (b - a).ort();
		}
		
		EquationLine(const Line<dimension> * line) {
			point = line->a;
			vector = (line->b - line->a).ort();
		}
		EquationLine(const Line<dimension> & line) {
			point = line.a;
			vector = (line.b - line.a).ort();
		}
		
		void printf(const char * format = "%07.3lf ") const {
			::printf("point  = "); point.printf(format);
			::printf("vector = "); vector.printf(format);
		}
		
		Vector<dimension> pointOnParam(double t) const {
			return point + t * vector;
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
		
		void printf(const char * format = "%07.3lf ") const {
			::printf("a = "); a.printf(printf);
			::printf("c = "); ::printf(format, c); 
		}
	};	
}

#endif