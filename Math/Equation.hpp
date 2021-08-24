#ifndef UF_M_Equation_H
#define UF_M_Equation_H

#include "../Utils/StdDiagnosticIgnore.hpp"

#include <initializer_list>

#include "../Utils/StdDiagnosticIgnoreEnd.hpp"

//UseFull Math Equation module
//Version 1.0 alfa
//Make by Graph Don'te-Crypto

#include "Vector.hpp"
#include "Shape.hpp"

namespace math {

	/*
		coords = point + t * vector, where

		point - base point of equation line,
		vector - equation line vector,
		t - any double number,
		coords(t) - any point from this equation line
	*/
	template <size_t dimension>
	struct EquationLine {

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

	private:
		EquationLine() {}
	public:

		static EquationLine Empty() {return EquationLine();}

		static EquationLine<dimension> fromPointAndVector(const Vector<dimension> & point, const Vector<dimension> & vector) {
			EquationLine<dimension> el = Empty();
			el.point = point;
			el.vector = vector.ort();
			return el;
		}

		static EquationLine<dimension> fromPointAndOrt(const Vector<dimension> & point, const Vector<dimension> & ort) {
			EquationLine<dimension> el = Empty();
			el.point = point;
			el.vector = ort;
			return el;
		}

		EquationLine(const Line<dimension> * line) {
			point = line->a;
			vector = (line->b - line->a).ort();
		}
		EquationLine(const Line<dimension> & line) :
			point(line.a),
			vector((line.b - line.a).ort())
		{}

		void printf(const char * format = "%07.3lf ") const {
			::printf("point  = "); point.printf(format);
			::printf("vector = "); vector.printf(format);
		}

		Vector<dimension> pointOnParam(double t) const {
			return point + t * vector;
		}

	};

	/*
		a[1] * x[1] + ... + a[n] * x[n] + c = 0, where

		n - dimension count,
		a - ort vector,
		x - point,
		c - constant
	*/
	template <size_t dimension>
	struct EquationHyperplane {
		
		Vector<dimension> ort;
		double c;
		EquationHyperplane() {

		}
		EquationHyperplane(const Vector<dimension> & vector, double xc)
			: ort(vector)
			, c(xc)
		{}
		EquationHyperplane(const EquationHyperplane<dimension> & e)
			: ort(e.ort)
			, c(e.c)
		{}

		EquationHyperplane(const Vector<dimension> & point, const Vector<dimension> & ort)
			: ort(ort)
			, c(- (point * ort))
		{}

		void printf(const char * format = "%07.3lf ") const {
			::printf("a = "); ort.printf(format);
			::printf("c = "); ::printf(format, c);
		}

		std::string toString(const char * format = "%07.3lf ") const {
			return "a = " + ort.toString(format) + " c = " + std::to_string(c);
		}

		double valueInPoint(const Vector<dimension> & point) const {
			double result = c;
			for (size_t i = 0; i < dimension; i++)
				result += point[i] * ort[i];
			return result;
		}
	};
}

#endif
