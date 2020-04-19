#ifndef UF_M_Shape_H
#define UF_M_Shape_H

#include "Vector.h"
#include "../Utils/Minmax.h"

//UseFull Math Shape module
//Version 1.0 alfa
//Make by Graph Don'te-Crypto

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
	
	struct Line2D {
		Vector<2> a, b;
		
		Line2D(const Vector<2> & xa, const Vector<2> & xb) {a  = xa; b  = xb;}
		
		Line2D & operator += (const Vector<2> & xy) {a += xy; b += xy; return *this;}
		Line2D & operator -= (const Vector<2> & xy) {a -= xy; b -= xy; return *this;}
		
		Line2D   operator -  (const Vector<2> & xy) const {return Line2D(a - xy, b - xy);}
		Line2D   operator +  (const Vector<2> & xy) const {return Line2D(a + xy, b + xy);}
		
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
			return center == circle.center && r == circle.r;
		}
		
	};
	template <size_t dimension>	
	struct Codir {
		Vector<dimension> left_up, right_down;
		Codir(const Vector<dimension> & a, const Vector<dimension> & b) {
			double min1, max1;
			
			for (size_t i = 0; i < dimension; i++) {
				minmax(a[i], b[i], min1, max1);
				left_up[i] = min1;
				right_down[i] = max1;
			}
		}
	};
	struct Rectangle2D {
		Vector<2> a, b, c, d;
	};
}

#endif