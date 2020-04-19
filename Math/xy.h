#ifndef UF_M_XY_H
#define UF_M_XY_H

#include <math.h>

//UseFull Math XY(x, y), RF(r, phi) and 2D Vector struct 
//Version 3.8 realize
//Make by Graph Don'te-Crypto

#define PI 3.1415926535897931
#define C const
#define D double

namespace math {
	struct RF;
	struct XY;

	struct RF {
		D r, phi;
		RF() : r(0), phi(0) {}
		RF & operator () (C D & a, C D & b) {r  = a; phi = b; return *this;}
		RF(C D & qr, C D & qphi) {r = qr; phi = qphi;}
		friend RF & operator *= (  RF &, C D  &);
		friend RF & operator /= (  RF &, C D  &);
		friend RF   operator *  (C RF &, C D  &);
		friend RF   operator /  (C RF &, C D  &);
		friend bool operator == (C RF &, C RF &);
		friend bool operator != (C RF &, C RF &);
		
		XY toXY() C;
	};
	struct XY {
		D x,y;
		XY () : x(0), y(0){}
		XY & operator () (C D  & qx1, C D  & qy1) {x = qx1;   y = qy1;  return *this;};
		XY & operator () (C XY & qx1, C D  & qy1) {x = qx1.x; y = qy1;  return *this;};
		XY & operator () (C D  & qx1, C XY & qy1) {x = qx1;   y = qy1.y; return *this;};
		XY & operator () (C XY & qx1, C XY & qy1) {x = qx1.x; y = qy1.y; return *this;};
		XY(C D & qx, C D & qy) {x = qx;  y = qy;}
		XY(C XY & q, C XY & w) {x = q.x; y = w.y;}
		XY(C D & qx, C XY & w) {x = qx; y = w.y;}
		XY(C XY & q, C D & qy) {x = q.x; y = qy;}

		friend XY   operator -  (C XY &);
		friend XY   operator *  (C XY &, C D  &);
		friend XY   operator *  (C D  &, C XY &);
		friend XY   operator /  (C XY &, C D  &);
		friend XY & operator *= (  XY &, C D  &);
		friend XY & operator /= (  XY &, C D  &);
		friend XY & operator += (  XY &, C XY &);
		friend XY & operator -= (  XY &, C XY &);
		friend XY   operator +  (C XY &, C XY &);
		friend XY   operator -  (C XY &, C XY &);
		friend bool operator == (C XY &, C XY &);
		friend bool operator != (C XY &, C XY &);
		friend bool operator >= (C XY &, C XY &);
		friend bool operator <= (C XY &, C XY &);
		friend bool operator >  (C XY &, C XY &);
		friend bool operator <  (C XY &, C XY &);

		double distanceTo(XY & xy) const {
			return sqrt((y - xy.y)*(y - xy.y) + (x - xy.x)*(x - xy.x));
		}
		
		double norm() const {
			return sqrt(x * x + y * y);
		}

		RF toRF() C;

	};
	XY toXY(C RF & n);
	RF toRF(C XY & n);


	XY RF::toXY() C {return XY(r*cos(phi), r*sin(phi));}
	RF XY::toRF() C {
		D r = norm();
		D angle = acos(x/r);
		if (r == 0) angle = 0;
		else if (y < 0) angle = 2*PI - angle;
		return RF(r, angle);
	}



	XY   operator -  (C XY & xy)              {return XY(-xy.x, -xy.y);}
	XY   operator +  (C XY & xy1, C XY & xy2) {return XY(xy1.x + xy2.x, xy1.y + xy2.y);}
	XY   operator -  (C XY & xy1, C XY & xy2) {return XY(xy1.x - xy2.x, xy1.y - xy2.y);}
	XY   operator *  (C XY & xy1, C D  & fd1) {return XY(xy1.x * fd1  , xy1.y * fd1  );}
	XY   operator *  (C D  & fd1, C XY & xy1) {return XY(xy1.x * fd1  , xy1.y * fd1  );}
	XY   operator /  (C XY & xy1, C D  & fd1) {return XY(xy1.x / fd1  , xy1.y / fd1  );}

	XY & operator += (  XY & xy1, C XY & xy2) {xy1.x += xy2.x;xy1.y += xy2.y;return xy1;}
	XY & operator -= (  XY & xy1, C XY & xy2) {xy1.x -= xy2.x;xy1.y -= xy2.y;return xy1;}
	XY & operator *= (  XY & xy1, C D  & fd1) {xy1.x *= fd1  ;xy1.y *= fd1  ;return xy1;}
	XY & operator /= (  XY & xy1, C D  & fd1) {xy1.x /= fd1  ;xy1.y /= fd1  ;return xy1;}
	bool operator == (C XY & xy1, C XY & xy2) {return (xy1.x == xy2.x) && (xy1.y == xy2.y);}
	bool operator != (C XY & xy1, C XY & xy2) {return (xy1.x != xy2.x) || (xy1.y != xy2.y);}
	bool operator >  (C XY & xy1, C XY & xy2) {return (xy1.x >  xy2.x) && (xy1.y >  xy2.y);}
	bool operator >= (C XY & xy1, C XY & xy2) {return (xy1.x >= xy2.x) && (xy1.y >= xy2.y);}
	bool operator <  (C XY & xy1, C XY & xy2) {return (xy1.x <  xy2.x) && (xy1.y <  xy2.y);}
	bool operator <= (C XY & xy1, C XY & xy2) {return (xy1.x <= xy2.x) && (xy1.y <= xy2.y);}


	RF & operator *= (  RF & rf1, C D  & qr1) {rf1.r *= qr1;return rf1;}
	RF & operator /= (  RF & rf1, C D  & qr1) {rf1.r /= qr1;return rf1;}
	RF   operator *  (C RF & rf1, C D  & qr1) {return RF(rf1.r * qr1, rf1.phi);}
	RF   operator /  (C RF & rf1, C D  & qr1) {return RF(rf1.r / qr1, rf1.phi);}
	bool operator == (C RF & rf1, C RF & rf2) {return (rf1.r == rf2.r) && (rf1.phi == rf2.phi);}
	bool operator != (C RF & rf1, C RF & rf2) {return (rf1.r != rf2.r) || (rf1.phi != rf2.phi);}
}
	
#undef C
#undef D

#endif