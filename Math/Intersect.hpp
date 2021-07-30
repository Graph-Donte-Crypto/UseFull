#ifndef UF_M_Intersect_H
#define UF_M_Intersect_H

#include "UseFull/Utils/Macro.hpp"
#include "UseFull/Utils/StdDiagnosticIgnore.hpp"

#include <cstddef>
#include <cstdlib>
#include <vector>

#include "UseFull/Utils/StdDiagnosticIgnoreEnd.hpp"

#include "Vector.hpp"
#include "Shape.hpp"
#include "../Utils/Ok.hpp"
#include "../Utils/Macro.hpp"
#include "Equation.hpp"

#define EPS 0.001
#define is_zero(d) (abs(d) < EPS)


//UseFull Math Intersect module
//Version 2.1 alfa
//Make by Graph Don'te-Crypto

//DM ~ DiMention

#define VDM Vector<DM>


namespace math {
	using namespace utils;
	
	/*   projection   */
		
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
		//eh.ort * point + eh.c = 0
		//eh.ort * el.point + t * el.vector * eh.ort + eh.c = 0
		//t = (-eh.c - eh.ort * el.point) / (el.vector * eh.ort)
		//point = el.point + ( (-eh.c - eh.ort * el.point) / (el->vector * eh.ort) ) * el.vector;
		
		/*
		
		EquationHyperplane<DM> eh(base, el.vector);
		double elv_eha = el.vector * eh.ort;
		if (abs(elv_eha) <= EPS) {
			printf("Equation Line special error\nbase:\n");
			base.printf();
			printf("el:\n");
			eh.ort.printf();
			printf("c: %lf\n", eh.c);
			return {};
		}
		//if _ret == {} -> something goes wrong;
		//return el.point + el.vector * ( (-eh.c - (eh.ort * el.point)) / elv_eha );
		return el.pointOnParam((-eh.c - (eh.ort * el.point)) / elv_eha);
		
		*/
		
		//Testing new version of projection algorithm
		//base - el.point - vector from el.point to base
		//el.vector * (base - el.point) - length of projection
		return el.pointOnParam(el.vector * (base - el.point));
	}
	//
	
	template<size_t DM>
	VDM intersectEquationLineWithHyperplane(const EquationLine<DM> & el, const EquationHyperplane<DM> & eh) {
		/*
			point * eh.ort + eh.c = 0
			el.point + k * el.vector = point
			(el.point + k * el.vector) * eh.ort = point * eh.ort
			el.point * eh.ort + (k * el.vector) * eh.ort + eh.c = 0
			el.point * eh.ort + k * (el.vector * eh.ort) + eh.c = 0

			k = - (eh.c + el.point * eh.ort) / (el.vector * eh.ort)

			el.point - ( (eh.c + el.point * eh.ort) / (el.vector * eh.ort) ) * el.vector = point

		*/
		
		return el.point - ( (eh.c + el.point * eh.ort) / (el.vector * eh.ort) ) * el.vector;
	}

	template <size_t DM>
	VDM reflectPointOverLine(const VDM & base, const EquationLine<DM> & el){
		return projectionPointOnEquationLine(base, el) * 2 - base;
	}

	template <size_t DM>
	VDM reflectPointOverHyperplane(const VDM & base, const EquationHyperplane<DM> & eh) {
		return projectionPointOnEquationHyperplane(base, eh) * 2 - base;
	}

	//Unimplimented
	template <size_t DM>
	VDM reflectPointOverHyperplaneOrt(const VDM & base, const EquationHyperplane<DM> & eh) {
		prefix_unused(base);
		prefix_unused(eh);
		printf("reflectPointOverHyperplaneOrt Unimplimented\n");
		exit(1);
	} 

	//TODO: CHECK
	template <size_t DM>
	VDM projectionPointOnEquationHyperplane(const VDM & base, const EquationHyperplane<DM> & eh) {
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
	Ok<VDM> & getFirstIntersect(const Line<DM> & line, const std::vector<Line<DM>> & lines) {
		
		Ok<VDM> result;
		double distance = 0;

		size_t i = 0;
		for (; i < lines.size(); i++) {
			result = intersectLineWithLine(line, lines[i]);
			if (result.isOk) {
				distance = line.a.distanceTo(result.value);
				break;
			}
		}

		Ok<VDM> new_result;

		for (; i < lines.size(); i++) {
			new_result = intersectLineWithCodir(line, lines[i]);
			if (new_result.isOk) {
				double new_distance = line.a.distanceTo(new_result.value);
				if (new_distance < distance) {
					result = new_result;
					distance = new_distance;
				}
			}
		}

		return result;
	}

	template <size_t DM>
	const VDM & nearestPointToPoint(const VDM & base, const VDM & one, const VDM & two) {
		/*
		(one - base).norm() < (two - base).norm()
		x = one - base
		y = two - base
		sqrt(x[1]^2 + ... + x[n]^2) < sqrt(y[1]^2 + ... + y[n]^2)
		x[1]^2 + ... + x[n]^2 < y[1]^2 + ... + y[n]^2
		x[1]^2 - y[1]^2 + ... + x[n]^2 - y[n]^2 < 0
		(x[1] + y[1])(x[1] - y[1]) + ... + (x[n] + y[n])(x[n] - y[n]) < 0

		(one[1] - base[1] + two[1] - base[1])(one[1] - base[1] - two[1] + base[1]) + ... < 0

		sum ((one[i] + two[i] - 2 * base[i]) * (one[i] - two[i])) < 0

		*/

		double sum = 0;
		for (size_t i = 0; i < DM; i++)
			sum += (one[i] + two[i] - 2 * base[i]) * (one[i] - two[i]);
		return sum < 0 ? one : two;
	}
	
	template <size_t DM>
	bool checkPointInCodir(const VDM & point, const Codir<DM> & codir, double eps = 0) {
		return (point > codir.left_up - VDM(eps)) && (point < codir.right_down + VDM(eps));
	}
	
	//TODO: CHECK
	template<size_t DM>
	Ok<VDM> projectionPointOnLine(const VDM & base, const Line<DM> & line) {
		Ok<VDM> projection = projectionPointOnEquationLine(base, EquationLine(line));
		if (projection.isOk && checkPointInCodir(projection.value, Codir(line), EPS)) 
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
	
	
	//TODO: CHECK
	template <size_t DM>
	Ok<VDM> intersectEquationLineWithEquationLine(const EquationLine<DM> & el1, const EquationLine<DM> & el2) {
		if (DM == 1) {
			printf("intersectEquationLineWithEquationLine with DM = 1 has no sense\n"); 
			exit(0);
		}
		
		const VDM & b1 = el1.vector;
		const VDM & b2 = el2.vector;
		const VDM    c = el1.point - el2.point;
		std::vector<size_t> indexes;
		
		for (size_t i = 0; i < DM; i++) { //find all indexes, where exists b1 != 0 or b2 != 0
			if (!is_zero(b1[i]) || !is_zero(b2[i])) indexes.push_back(i);
			else if (!is_zero(c[i])) return {}; //when b1 = 0, b2 = 0 => must c = 0
		}
		
		if (indexes.size() == 0) return el1.point;
		
		const size_t & i = indexes[0];
		
		if (indexes.size() == 1) {
			if (!is_zero(b1[i]) && !is_zero(b2[i])) {
				printf("Infinity Many solutions?\n");
				return {};
			}
			//TODO: if indexes.size() == 1 and not if, indexes[1] cause error
		}
		
		const size_t & i_next = indexes[1];
		
		double k1 = 0, k2 = 0;
		
		/*
		Main_Equation:
			k2 * b2 - k1 * b1 = c
		*/
		
		size_t j = 1;
		
		if (is_zero(b2[i])) {
			k1 = c[i] / (- b1[i]); //from Main_Equation
			
			for (; j < indexes.size(); j++) {
				const size_t & i_cur = indexes[j];
				if (is_zero(b2[i_cur])) { //when b2 = 0, check k1 for equality
					if (!is_zero( k1 + c[i_cur] / b1[i_cur] )) return {};
				}
				else { //else, find k2
					k2 = (b1[i_cur] * k1 + c[i_cur]) / b2[i_cur];
					break;
				}
			}
		}
		else if (is_zero(b1[i])) {
			k2 = c[i] / b2[i]; //from Main_Equation
			
			for (; j < indexes.size(); j++) {
				const size_t & i_cur = indexes[j];
				if (is_zero(b1[i_cur])) { //when b1 = 0, check k2 for equality
					if (!is_zero( k2 - c[i_cur] / b2[i_cur] )) return {};
				}
				else { //else, find k1
					k1 = (b2[i_cur] * k2 - c[i_cur]) / b1[i_cur];
					break;
				}
			}
			
		}
		else {
			//solve equations system size 2
			double del = b2[i_next] * b1[i] - b1[i_next] * b2[i];
			if (abs(del) < EPS) return {};
				
			k1 = (c[i_next] * b2[i] - c[i] * b2[i_next]) / del;
			k2 = (c[i] + b1[i] * k1) / b2[i];
			
			j = 2;
		}
		
		for (; j < indexes.size(); j++) {
			const size_t & i_cur = indexes[j];
			if (!is_zero(c[i_cur] - k2 * b2[i_cur] + k1 * b1[i_cur])) return {};
		}
		
		return el1.pointOnParam(k1);
		
	}
	
	//TODO: CHECK
	template <size_t DM>
	Ok<VDM> intersectLineWithLine(const Line<DM> & line1, const Line<DM> & line2) {
		Ok<VDM> res = intersectEquationLineWithEquationLine(EquationLine<DM>(line1), EquationLine<DM>(line2));
		if (res.isOk) {
			if (!checkPointInCodir(res.value, Codir<DM>(line1))) return {};
			if (!checkPointInCodir(res.value, Codir<DM>(line2))) return {};
			return res.value;
		}
		else return {};
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
	bool checkIntersectCodirWithCodir(const Codir<DM> & c1, const Codir<DM> & c2, double eps = 0) {
		return checkPointInCodir(c1.left_up, Codir<DM>(c2.left_up - c1.right_down + c1.left_up, c2.right_down), eps);
	}
	//
	
	//TODO : CHECK
	template <size_t DM>
	bool checkIntersectSphereWithSphere(const Sphere<DM> & s1, const Sphere<DM> & s2) {
		return (s2.center - s1.center).norm() >= s1.r + s2.r;
	}
	//
}

#undef VDM 
#undef EPS

#endif
