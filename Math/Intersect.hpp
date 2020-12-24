#ifndef UF_M_Intersect_H
#define UF_M_Intersect_H

#include "Vector.hpp"
#include "Shape.hpp"
#include "../Utils/Ok.hpp"
#include "Equation.hpp"

#define EPS 0.001

//UseFull Math Intersect module
//Version 2.0 alfa
//Make by Graph Don'te-Crypto

//DM ~ DiMention

#define VDM Vector<DM>


namespace math {
	
	using namespace utils;
	
	template <size_t DM>
	VDM projectionPointOnSphere(const VDM * base, const Sphere<DM> * sphere) {
		return (*base - sphere->center).ort() * sphere->r + sphere->center;
	}
	
	template <size_t DM>
	VDM projectionPointOnSphere(const VDM & base, const Sphere<DM> & sphere) {
		return projectionPointOnSphere(&base, &sphere);
	}
	
	//TODO: CHECK
	template <size_t DM>
	Ok<VDM> projectionPointOnEquationLine(const VDM & base, const EquationLine<DM> & el) {
		//point = el.point + t * el.vector;
		//eh.a * point + eh.c = 0
		//eh.a * el.point + t * el.vector * eh.a + eh.c = 0
		//t = (-eh.c - eh.a * el.point) / (el.vector * eh.a)
		//point = el.point + ( (-eh.c - eh.a * el.point) / (el->vector * eh.a) ) * el.vector;
		EquationHyperplane<DM> eh(base, el.vector);
		double elv_eha = el.vector * eh.ort;
		if (abs(elv_eha) <= EPS) return {};
		//if _ret == {} -> something goes wrong;
		//return el.point + el.vector * ( (-eh.c - (eh.ort * el.point)) / elv_eha );
		return el.pointOnParam((-eh.c - (eh.ort * el.point)) / elv_eha);
	}
	//
	
	//TODO: CHECK
	template <size_t DM>
	Ok<VDM> projectionPointOnEquationHyperplane(const VDM & base, const EquationHyperplane<DM> & eh) {
		/* 
			B - result point
		
			x = eh.ort[0] * base[0] + ... + eh.ort[n-1] * base[n-1] + eh.c 
			0 = eh.ort[0] * B[0]    + ... + eh.ort[n-1] * B[n-1]    + eh.c 
			
			x - 0 = eh.ort[0] * (base[0] - B[0]) + ... eh.ort[n-1] * (base[n-1] - B[n-1])
			
			(base + k * eh.ort = B) - line by base and ort. k - double
			
			base = B - k * eh.ort
			base[i] = B[i] - k * eh.ort[i]
			=>
			
			x = eh.ort[0] * (B[0] - k * eh.ort[0] - B[0]) + ... eh.ort[n-1] * (B[n-1] - k * eh.ort[n-1] - B[n-1])
			x = eh.ort[0] * (- k * eh.ort[0]) + ... eh.ort[n-1] * (- k * eh.ort[n-1])
			x = (-k) * eh.ort[0]^2 + ... + (-k) * eh.ort[n-1]^2
			x = -k * (eh.ort[0]^2 + ... + eh.ort[n-1]^2)
			k = -x / (eh.ort[0]^2 + ... + eh.ort[n-1]^2)
			
			B = base + k * eh.ort = base + eh.ort * (-x / (eh.ort[0]^2 + ... + eh.ort[n-1]^2))
			
		*/
		
		double x = eh.valueInPoint(base);
		double ort2_sum = 0;
		for (size_t i = 0; i < DM; i++)
			ort2_sum += eh.ort[i] * eh.ort[i];
		
		return base - eh.ort * (x / ort2_sum);
	}
	//
	
	template <size_t DM>
	const VDM & nearestPointToPoint(const VDM & base, const VDM & one, const VDM & two) {
		return (one - base).norm() < (two - base).norm() ? one : two;
	}
	
	template <size_t DM>
	bool checkPointInCodir(const VDM & point, const Codir<DM> & codir, double eps = 0) {
		return (point > codir.left_up - VDM(eps)) && (point < codir.right_down + VDM(eps));
	}
	
	
		
	//TODO: CHECK
	template<size_t DM>
	Ok<VDM> projectionPointOnLine(const VDM & base, const Line<DM> & line) {
		Ok<VDM> projection = projectionPointOnEquationLine(base, line);
		if (projection.isOk && checkPointInCodir(projection.value, line, EPS)) 
			return projection;
		else return {};
	}
	//
	
	//TODO: CHECK
	template <size_t DM>
	bool checkIntersectPointWithEquationLine(const VDM & point, const EquationLine<DM> & e) {
		//return abs(e.a * xy.x + e.b * xy.y + e.c) < EPS;
		double t = 0;
		for (size_t i = 0; i < DM; i++) {
			if (abs(e.vector[i]) > EPS) {
				t = (point[i] - e.point[i]) / e.vector[i];
				break;
			}
		}
		
		if ( (point - e.pointOnParam(t)).getAbsVec() <= EPS ) return true;
		else return false;
		/*
		for (size_t i = 0; i < DM; i++) {
			if (abs(point[i] - e.point[i] - t * e.vector[i]) > EPS) return false;
		}
		return true;
		*/
	}
	//
	
	template <size_t DM>
	bool checkIntersectPointWithLine(const VDM & point, const Line<DM> & line) {
		if (checkIntersectPointWithEquationLine(point, EquationLine<DM>(line)))
			return checkPointInCodir(point, Codir<DM>(line));
		return false;
	}
	
	bool checkIntersectEquationLineWithEquationLine2D(const EquationLine<2> & el0, const EquationLine<2> & el1) {
		if (abs(el1.vector[1] * el0.vector[0] - el0.vector[1] * el1.vector[0]) < EPS) return false;
		else return true;
	}
	
	Ok<Vector<2>> intersectEquationLineWithEquationLine2D(const EquationLine<2> & el0, const EquationLine<2> & el1) {		
		Vector<2> c = el0.point - el1.point;
		
		double del = el1.vector[1] * el0.vector[0] - el0.vector[1] * el1.vector[0];
		
		if (abs(del) < EPS) return {};

		return el0.pointOnParam((el1.vector[0] * c[1] - el1.vector[1] * c[0]) / del);
		
		/*
		//Strange code
		return Vector<2>({
			el0.pointOnParam((el1.vector[1] * c[1] - el1.vector[0] * c[0]) / del),
			el1.pointOnParam((el0.vector[1] * c[1] - el0.vector[0] * c[0]) / del)
		});
		*/
	}
		
	Ok<Vector<2>> intersectLineWithLine2D(const Line<2> & l0, const Line<2> & l1) {
		Ok<Vector<2>> mb_point = intersectEquationLineWithEquationLine2D(l0, l1);
		if (mb_point.isOk && 
			checkPointInCodir<2>(mb_point, l0, EPS) && 
			checkPointInCodir<2>(mb_point, l1, EPS)
			) return mb_point;
		return {};
	}
		
	template <size_t DM>
	double distanceBeetweenPointAndLineMin(const VDM & v, const Line<DM> & l) {
		Ok<VDM> proj = projectionPointOnLine(v, l);
		if (proj.isOk) return v.distanceTo(proj.value);
		else return min(v.distanceTo(l.a), v.distanceTo(l.b));
	}
	
	template <size_t DM>
	double distanceBeetweenPointAndLineMax(const VDM & v, const Line<DM> & l) {
		return max(v.distanceTo(l.a), v.distanceTo(l.b));
	}
		
		
	template <size_t DM>
	double distanceBeetweenLines(const Line<DM> & l1, const Line<DM> & l2) {
		Ok<VDM> proj[4] = {
			projectionPointOnLine(l1.a, l2),
			projectionPointOnLine(l1.b, l2),
			projectionPointOnLine(l2.a, l1),
			projectionPointOnLine(l2.b, l1)
		};
		bool is_ok = proj[0].isOk || proj[1].isOk || proj[2].isOk || proj[3].isOk;
		if (is_ok) {
			
		}
		return min(
			l1.a.distanceTo(l2.a),
			l1.a.distanceTo(l2.b),
			l1.b.distanceTo(l2.a),
			l1.b.distanceTo(l2.b)
		);
	}
	
	
	//TODO: RELEASE
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
	//
	
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
	}
		
	template <size_t DM>
	std::pair<Ok<VDM>, Ok<VDM>> intersectEquationLineWithCodir(const EquationLine<DM> & el, const Codir<DM> & c) {
		double mins[DM], maxs[DM];
		size_t count = 0;
		for (size_t i = 0; i < DM; i++) {
			if (el.vector[i] > 0) {
				mins[count] = (c.left_up[i]    - el.point[i]) / el.vector[i];
				maxs[count] = (c.right_down[i] - el.point[i]) / el.vector[i];
				count += 1;
			}
			else if (el.vector[i] < 0){
				maxs[count] = (c.left_up[i]    - el.point[i]) / el.vector[i];
				mins[count] = (c.right_down[i] - el.point[i]) / el.vector[i];
				count += 1;
			}
			else {
				if (c.left_up[i] - el.point[i] > 0 || c.right_down[i] - el.point[i] < 0)
					return std::pair<Ok<VDM>, Ok<VDM>>({}, {});
				/* OOF SYKA */
			}
		}
		double min = mins[0], max = maxs[0];
		for (size_t i = 1; i < count; i++) {
			if (mins[i] > min) min = mins[i];
			if (maxs[i] < max) max = maxs[i];
		}
		if (min > max) 
			return std::pair<Ok<VDM>, Ok<VDM>>({}, {});
		else if (abs(min - max) < EPS) 
			return std::pair<Ok<VDM>, Ok<VDM>>(el.pointOnParam( (min + max) / 2), {});
		else 
			return std::pair<Ok<VDM>, Ok<VDM>>(el.pointOnParam(min), el.pointOnParam(max));
		
	}
	
	template <size_t DM>
	std::pair<Ok<VDM>, Ok<VDM>> intersectLineWithCodir(const Line<DM> & l, const Codir<DM> & c) {
		if ((l.a - l.b).getAbsVec() < EPS) return std::pair<Ok<VDM>, Ok<VDM>>({}, {});
		std::pair<Ok<VDM>, Ok<VDM>> pair = intersectEquationLineWithCodir(EquationLine<DM>(l), c);
		bool active1 = pair.first.isOk  && checkPointInCodir<2>(pair.first.value , l, EPS);
		bool active2 = pair.second.isOk && checkPointInCodir<2>(pair.second.value, l, EPS);
		Ok<VDM> ret1 = {}, ret2 = {};
		if (active1) ret1 = pair.first;
		if (active2) ret2 = pair.second;
		return std::pair<Ok<VDM>, Ok<VDM>>(ret1, ret2);
	}
	
	//TODO: CHECK
	template <size_t DM>
	bool checkIntersectCodirWithCodir(const Codir<DM> & c1, const Codir<DM> & c2) {
		return checkPointInCodir(c1.left_up, Codir<DM>(c2.left_up - c1.right_down + c1.left_up, c2.right_down));
	}
	//
	
	//TODO : RELEASE
	template <size_t DM>
	bool checkIntersectSphereWithSphere(const Sphere<DM> & s1, const Sphere<DM> & s2) {
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