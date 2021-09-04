#ifndef UF_M_Shape_H
#define UF_M_Shape_H

#include "../Utils/StdDiagnosticIgnore.hpp"

#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <tuple>

#include "../Utils/StdDiagnosticIgnoreEnd.hpp"

//UseFull Math Shape module
//Version 1.0 alfa
//Make by Graph Don'te-Crypto

#include "Vector.hpp"
#include "../Utils/Minmax.hpp"

#define EPS 0.001

namespace math {
	
	using namespace utils;
	
	enum class LengthenWay {
		TowardsA,
		TowardsB,
		Equally
	};
	
	template <size_t dimension>
	struct Line {
		Vector<dimension> a, b;
		
		Line(const Vector<dimension> & xa, const Vector<dimension> & xb) {a  = xa; b  = xb;}
		Line() : a(0), b(0) { }
		
		Line & operator += (const Vector<dimension> & vec) {a += vec; b += vec; return *this;}
		Line & operator -= (const Vector<dimension> & vec) {a -= vec; b -= vec; return *this;}
		
		Line   operator -  (const Vector<dimension> & vec) const {return Line(a - vec, b - vec);}
		Line   operator +  (const Vector<dimension> & vec) const {return Line(a + vec, b + vec);}
		
		void printf(const char * format = "%07.3lf ") const {
			::printf("A: "); a.printf(format);
			::printf("B: "); b.printf(format);
		}

		std::string toString(const char * format = "%07.3lf ") {
			return "A: " + a.toString(format) + "\nB: " + b.toString(format);
		}
		
		Line & lengthenBy(double value, LengthenWay way = LengthenWay::Equally) {
			Vector<dimension> ort = (b - a).ort();
			switch (way) {
			case LengthenWay::TowardsA: 
				a -= ort * value;
				break;
			case LengthenWay::TowardsB: 
				b += ort * value;
				break;
			case LengthenWay::Equally: 
				b += ort * value;
				a -= ort * value;
				break;
			default:
				::printf("Vector::lengthenBy not implimented\n");
				exit(1);
			}
			return *this;
		}
	};
	
	template <size_t dimension>
	struct Sphere {
		Vector<dimension> center;
		double r;
		Sphere() : center(0), r(0) {}
		Sphere(const Vector<dimension> & xcenter, double xr) : center(xcenter), r(xr){}
		
		bool operator == (const Sphere<dimension> & circle) {
			return center == circle.center && abs(r - circle.r) < EPS;
		}
		void printf(const char * format = "%07.3lf ") const {
			::printf("Center: "); center.printf(format);
			::printf("Radius: "); ::printf(format, r);
			::printf("\n");
		}

		std::string toString(const char * format = "%07.3lf ") const {
			return "Center " + center.toString(format) + "\nRadius: " + std::to_string(r);
		}

		Sphere & operator += (const Vector<dimension> & vec) 
			{center += vec; return *this;}
		Sphere & operator -= (const Vector<dimension> & vec) 
			{center -= vec; return *this;}
		Sphere   operator +  (const Vector<dimension> & vec) const 
			{return Sphere(center + vec, r);}
		Sphere   operator -  (const Vector<dimension> & vec) const 
			{return Sphere(center - vec, r);}
	};
	/*
	Прямоугольное тело, рёбра которого соответственно параллельны осям координат
	Rectangular body, which's edges are respectively parallel to the coordinate axes
	*/
	template <size_t dimension>	
	struct Codir {
		Vector<dimension> left_up = Vector<dimension>(), right_down = Vector<dimension>();
		Codir() {}
		
		//From size
		Codir (const Vector<dimension> & right_down) {
			left_up = 0;
			this->right_down = right_down;
		}
		//From size
		static Codir fromSize(const Vector<dimension> & right_down) {
			return Codir<dimension>(Vector<dimension>(0), right_down);
		}
		//From two points
		Codir(const Vector<dimension> & a, const Vector<dimension> & b) {
			for (size_t i = 0; i < dimension; i++) 
				std::tie(left_up[i], right_down[i]) = minmax(a[i], b[i]);
		}
		//From line
		Codir(const Line<dimension> & line) {
			for (size_t i = 0; i < dimension; i++) 
				std::tie(left_up[i], right_down[i]) = minmax(line.a[i], line.b[i]);
		}
		Codir(const Codir<dimension> & codir) :
			left_up(codir.left_up),
			right_down(codir.right_down) {}
		
		Codir & operator = (const Codir & codir) {
			left_up    = codir.left_up;
			right_down = codir.right_down;
			return *this;
		}
		
		void printf(const char * format = "%07.3lf ") const {
			::printf("LU: "); left_up.printf(format);
			::printf("RD: "); right_down.printf(format);
		}
		Vector<dimension> center() const {
			return (left_up + right_down) / 2;
		}
		Vector<dimension> size() const {
			return right_down - left_up;
		}
		Codir & operator += (const Vector<dimension> & vec) {left_up += vec; right_down += vec; return *this;}
		Codir & operator -= (const Vector<dimension> & vec) {left_up -= vec; right_down -= vec; return *this;}
		
		Codir   operator -  (const Vector<dimension> & vec) const {return Codir(*this) -= vec;}
		Codir   operator +  (const Vector<dimension> & vec) const {return Codir(*this) += vec;}

		Codir & operator *= (double d) {left_up *= d; right_down *= d; return *this;}
		Codir & operator /= (double d) {left_up /= d; right_down /= d; return *this;}

		Codir   operator *  (double d) const {return Codir(*this) *= d;}
		Codir   operator /  (double d) const {return Codir(*this) /= d;}

		std::string toString(const char * format = "%07.3lf ") {
			return "left_up " + left_up.toString(format) + "\nright_down " + right_down.toString(format);
		}
	};
	struct Rectangle2D {
		Vector<2> a, b, c, d;
	};
}

#undef EPS

#endif
