#ifndef UF_M_Intersect_H
#define UF_M_Intersect_H

#include "Vector.hpp"
#include "Shape.hpp"
#include "../Utils/Ok.hpp"
#include "Equation.hpp"

#define EPS 0.001

//UseFull Math Intersect module
//Version 1.0 alfa
//Make by Graph Don'te-Crypto

//DM ~ DiMention

#define VDM Vector<DM>


namespace math {
	
	template <size_t DM>
	VDM projectionPointOnSphere
		(const VDM * base, const Sphere<DM> * sphere) {
		return (*base - sphere->center).ort() * sphere->r + sphere->center;
	}
	
	template <size_t DM>
	VDM projectionPointOnSphere
		(const VDM & base, const Sphere<DM> & sphere) {
		return projectionPointOnSphere(&base, &sphere);
	}
	
	template <size_t DM>
	VDM projectionPointOnEquationLine
		(const VDM * base, const EquationLine<DM> * line) {
		//point = line->point + t * line->vector;
		//eh.a * point + eh.c = 0
		//eh.a * line->point + t * line->vector * eh.a + eh.c = 0
		//t = (-eh.c - eh.a * line->point) / (line->vector * eh.a)
		//point = line->point + ( (-eh.c - eh.a * line->point) / (line->vector * eh.a) ) * line->vector;
		EquationHyperplane<DM> eh(*base, line->vector);
		for (size_t i = 0; i < DM; i++) printf("a[%i](%lf) ", i, eh.a[i]);
		printf("c(%lf)\n", eh.c);
		return line->point + ( (-eh.c - (eh.a * line->point)) / (line->vector * eh.a) ) * line->vector;
	}
	
	template <size_t DM>
	void nearestPointToPoint
		(const VDM & base, const VDM & one, const VDM & two, VDM & ans) {
		ans = (one - base).norm() < (two - base).norm() ? one : two;
	}
	
	template <size_t DM>
	bool checkPointInCodir(const VDM & point, const Codir<DM> & codir) {
		return point > codir.left_up && point < codir.right_down;
	}
	
	template <size_t DM>
	bool intersectPointWithEquationLine(const VDM & point, const EquationLine<DM> & e) {
		//return abs(e.a * xy.x + e.b * xy.y + e.c) < EPS;
		double t = 0;
		for (size_t i = 0; i < DM; i++) {
			if (abs(e.vector[i]) > EPS) {
				t = (point[i] - e.point[i]) / e.vector[i];
				break;
			}
		}
		for (size_t i = 0; i < DM; i++) {
			if (abs(point[i] - e.point[i] - t * e.vector[i]) > EPS) return false;
		}
		return true;
	}
	
	template <size_t DM>
	bool intersectPointWithLine(const VDM & point, const Line<DM> & line) {
		if (intersectPointWithEquationLine(point, EquationLine<DM>(line)))
			return checkPointInCodir(point, Codir<DM>(line.a, line.b));
		return false;
	}
	
	template <size_t DM>
	Ok<VDM> intersectLineWithLine(const Line<DM> & line1, const Line<DM> & line2) {
		//X = line1_e.point + t1 * line1_e.vector;
		//X = line2_e.point + t2 * line2_e.vector;
		/*
		{line1_e.vector, 0, line1_e.point - X},
		{0, line1_e.vector, line2_e.point - X}
		
		->
		0 = (line1_e.point - line2_e.point) + t1 * line1_e.vector - t2 * line2_e.vector;
		
		*/
		return {};
		/*
		EquationLine e1(line1);
		EquationLine e2(line2);
		
		double del = e1.a * e2.b - e2.a * e1.b;
		if (del == 0) return {};
		else return VDM((e1.b * e2.c - e2.b * e1.c) / del, (e1.c * e2.a - e2.c * e1.a) / del);
		*/
	}
	template <size_t DM>
	std::pair<Ok<VDM>, Ok<VDM>> intersectLineWithSphere
		(const Line<DM> & line, const Sphere<DM> & sphere) {
		Line<DM> line_0(line - sphere.center);
		EquationLine<DM> e(&line_0);
		double a = e.vector * e.vector;
		double b = e.point  * e.vector * 2;
		double c = e.point  * e.point - sphere.r * sphere.r;
		double d = b * b - 4 * a * c;
		if (d <= - EPS) 
			return std::pair<Ok<VDM>, Ok<VDM>>({}, {});
		else if (abs(d) < EPS) {
			Ok<VDM> ans = e.pointOnParam( - b / (2 * a)) + sphere.center;
			if (!checkPointInCodir(ans.value, Codir<DM>(line))) ans.isOk = false;
			return std::pair<Ok<VDM>, Ok<VDM>>(ans, {});
		}
		else {
			Ok<VDM> ans0 = e.pointOnParam( (- b + sqrt(d))/ (2 * a)) + sphere.center;
			Ok<VDM>	ans1 = e.pointOnParam( (- b - sqrt(d))/ (2 * a)) + sphere.center;
			
			if (!checkPointInCodir(ans0.value, Codir<DM>(line))) ans0.isOk = false;
			if (!checkPointInCodir(ans1.value, Codir<DM>(line))) ans1.isOk = false;
			
			return std::pair<Ok<VDM>, Ok<VDM>>(ans0, ans1);
		}
		
			/*
		Line<DM> line_d = line - sphere.center;
		EquationLine<DM> e(line_d);
		//Mb EquationLine e(line - circle.center);
		
		double del = e.a * e.a + e.b * e.b;
		if (abs(e.c) > circle.r * sqrt(del) + EPS) return std::pair<Ok<Vector<2>>, Ok<Vector<2>>>({}, {});
		double x0 = - e.a * e.c / del,  y0 = - e.b * e.c / del;
		RectangleCodir recod(line.a, line.b);
		if (abs(abs(e.c) - circle.r * sqrt(del)) < EPS) {
			Vector<2> temp({x0, y0});
			//if point on endless line and in rectangle, created by line points, point on line
			if (checkPointInRectangleCodir(temp, recod)) {
				return std::pair<Ok<Vector<2>>, Ok<Vector<2>>>(temp + circle.center, {});
			} 
			else {
				return std::pair<Ok<Vector<2>>, Ok<Vector<2>>>({}, {});
			}
		} 
		else {
			double d = circle.r * circle.r - e.c * e.c / del;
			double mult = sqrt (d / del);
			Vector<2> temp({x0 + e.b * mult, y0 - e.a * mult});
			std::pair<Ok<Vector<2>>, Ok<Vector<2>>> ret;
			if (checkPointInRectangleCodir(temp, recod)) {
				ret.first = temp + circle.center;
				temp(x0 - e.b * mult, y0 + e.a * mult);
				if (checkPointInRectangleCodir(temp, recod)) {
					ret.second = temp + circle.center;
				}
			}
			else {
				temp(x0 - e.b * mult, y0 + e.a * mult);
				if (checkPointInRectangleCodir(temp, recod)) {
					ret.first = temp + circle.center;
				}
			}
			return ret;
		}
		*/
		/*
		
		(x - xc)^2 + (y - yc)^2 = r^2
		(x - x1) / (x2 - x1) = (y - y1) / (y2 - y1)
		
		//Перенос центра окружности на (0, 0)
		
		(x - xc + xc)^2 + (y - yc + yc)^2 = r^2
		x^2 + y^2 = r^2
		(x - x1 + xc) / (x2 - x1  + 2*xc) = (y - y1 + yc) / (y2 - y1 + 2*yc)
		*/
	}
	
	//TODO: CHECK
	template <size_t DM>
	std::pair<Ok<VDM>, Ok<VDM>> intersectEquationLineWithCodir(const EquationLine<DM> & el, const Codir<DM> & c) {
		double mins[DM], maxs[DM];
		for (size_t i = 0; i < DM; i++) {
			mins[DM] = (c.left_up[i]    - el.point[i]) / el.vector[i];
			maxs[DM] = (c.right_down[i] - el.point[i]) / el.vector[i];
		}
		double min = mins[0], max = maxs[0];
		for (size_t i = 1; i < DM; i++) {
			if (mins[i] > min) min = mins[i];
			if (maxs[i] < max) max = maxs[i];
		}
		if (min > max) return std::pair<Ok<VDM>, Ok<VDM>>({}, {});
		else if (abs(min - max) < EPS) {
			return std::pair<Ok<VDM>, Ok<VDM>>(el.point + (min + max) * el.vector / 2, {});
		}
		else {
			return std::pair<Ok<VDM>, Ok<VDM>>(
				el.point + min * el.vector,
				el.point + max * el.vector
			);
		}
	}
	//
	//TODO: REMAKE
	//TODO: CHECK
	std::pair<Ok<VDM>, Ok<VDM>> intersectLineWithCodir(const Line<DM> & l, const Codir<DM> & c) {
		std::pair<Ok<VDM>, Ok<VDM>> pair = intersectEquationLineWithCodir(EquationLine<DM>(l), c));
		bool active1 = pair.first.isOk  && checkPointInCodir(pair.first, c);
		bool active2 = pair.second.isOk && checkPointInCodir(pair.second, c);
		if (active2 && !active1) return std::pair<Ok<VDM>, Ok<VDM>>(pair.second, {});
		return std::pair<Ok<VDM>, Ok<VDM>>(
			active1 ? pair.first : {}, 
			active2 ? pair.second : {}
		);
	}
	//
	//TODO : RELEASE
	template <size_t DM>
	bool intersectSphereWithSphere(const Sphere<DM> & s1, const Sphere<DM> & s2) {
		/*
		
		Если сфера 
		
		*/
		return {};
	}
	//
}

#undef VDM 
#undef EPS

#endif