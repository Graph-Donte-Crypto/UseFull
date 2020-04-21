#ifndef UF_M_Shape_H
#define UF_M_Shape_H

#include "Vector.h"
#include "../Utils/Minmax.h"

//UseFull Math Shape module
//Version 1.0 alfa
//Make by Graph Don'te-Crypto

#define EPS 0.001

namespace math {
	template <size_t dimension>
	struct Line {
		Vector<dimension> a, b;
		
		Line(const Vector<dimension> & xa, const Vector<dimension> & xb) {a  = xa; b  = xb;}
		
		Line & operator += (const Vector<dimension> & xy) {a += xy; b += xy; return *this;}
		Line & operator -= (const Vector<dimension> & xy) {a -= xy; b -= xy; return *this;}
		
		Line   operator -  (const Vector<dimension> & xy) const {return Line(a - xy, b - xy);}
		Line   operator +  (const Vector<dimension> & xy) const {return Line(a + xy, b + xy);}
		
	};
	
	template <size_t dimension>
	struct Sphere {
		Vector<dimension> center;
		double r;
		Sphere() {
			center = 0;
			r = 0;
		}
		Sphere(const Vector<dimension> & xcenter, double xr) {center = xcenter; r = xr;}
		bool operator == (const Sphere<dimension> & circle) {
			return center == circle.center && abs(r - circle.r) < EPS;
		}
		
	};
	template <size_t dimension>	
	struct Codir {
		Vector<dimension> left_up, right_down;
		Codir(const Vector<dimension> & a, const Vector<dimension> & b) {
			for (size_t i = 0; i < dimension; i++) 
				minmax(a[i], b[i], left_up[i], right_down[i]);
		}
		Codir(const Line<dimension> & line) {
			for (size_t i = 0; i < dimension; i++) 
				minmax(line.a[i], line.b[i], left_up[i], right_down[i]);
		}
	};
	struct Rectangle2D {
		Vector<2> a, b, c, d;
	};
}

#undef EPS

#endif