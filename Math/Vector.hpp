#ifndef UF_M_Vector_H
#define UF_M_Vector_H

#include <math.h>
#include <string.h>
#include <stdio.h>
#include <initializer_list>

//UseFull Math Vector template dimension module
//Version 1.0 alfa
//Make by Graph Don'te-Crypto

#define PI 3.1415926535897931
#define C const
#define D double
#define EPS 0.001

namespace math {
	template <size_t dimension>
	struct Vector;
	
	template <size_t width, size_t height>
	struct Matrix;
	
	template <size_t dimension>
	struct Vector {
		D coords[dimension];
		Vector () {}
		Vector (C D & d) {
			for (size_t i = 0; i < dimension; i++) coords[i] = d;
		}
		Vector (const Vector<dimension> & vector) {
			memcpy(coords, vector.coords, dimension * sizeof(D));
		}
		Vector(std::initializer_list<D> list) {
			if (list.size() != dimension) {
				::printf("Error: dimension = %lu, but list dimension = %lu\n", (unsigned long)dimension, (unsigned long)list.size());
				exit(1);
			}
			memcpy(coords, list.begin(), dimension * sizeof(D));
		}
		
		void printf(C char * format = "%07.3lf ") C {
			for (size_t i = 0; i < dimension; i++) ::printf(format, coords[i]);
			::printf("\n");
		}
		
		inline D   operator [] (size_t index) C {return coords[index];}
		inline D & operator [] (size_t index)   {return coords[index];}
		
		double operator *  (C Vector<dimension> & v1) C {
			double ans = 0;
			for (size_t i = 0; i < dimension; i++) ans += coords[i] * v1[i];
			return ans;
		}
		
		Vector<dimension>   operator -  () C {
			Vector<dimension> ret;
			for (size_t i = 0; i < dimension; i++) ret[i] = - coords[i];
			return ret;
		}
		
		Vector<dimension>   operator *  (C D  & d1) C {
			return Vector<dimension>(*this) *= d1;
		}
		
		Vector<dimension>   operator /  (C D  & d1) C {
			return Vector<dimension>(*this) /= d1;
		}
		
		Vector<dimension>   operator +  (C Vector<dimension> & p1) C {
			return Vector<dimension>(*this) += p1;
		}
		
		Vector<dimension>   operator -  (C Vector<dimension> & p1) C {
			return Vector<dimension>(*this) -= p1;
		}
		
		Vector<dimension> & operator *= (C D  & d1) {
			for (size_t i = 0; i < dimension; i++) coords[i] *= d1;
			return *this;
		}
		
		Vector<dimension> & operator /= (C D  & d1) {
			for (size_t i = 0; i < dimension; i++) coords[i] /= d1;
			return *this;
		}
		
		Vector<dimension> & operator += (C Vector<dimension> & p1) {
			for (size_t i = 0; i < dimension; i++) coords[i] += p1[i];
			return *this;
		}
		
		Vector<dimension> & operator -= (C Vector<dimension> & p1) {
			for (size_t i = 0; i < dimension; i++) coords[i] -= p1[i];
			return *this;
		}
		
		bool compareHard(C Vector<dimension> & p1) C {
			return (!memcmp(coords, p1.coords, dimension * sizeof(D)));
		}
		
		bool operator == (C Vector<dimension> & p1) C {
			for (size_t i = 0; i < dimension; i++) 
				if ( abs(coords[i] - p1[i]) > EPS) return false;
			return true;
		}
		
		bool operator != (C Vector<dimension> & p1) C {
			for (size_t i = 0; i < dimension; i++) 
				if ( abs(coords[i] - p1[i]) > EPS) return true;
			return false;
		}
		
		bool operator >= (C Vector<dimension> & p1) C {
			for (size_t i = 0; i < dimension; i++) if (coords[i] < p1[i]) return false;
			return true;
		}
		
		bool operator <= (C Vector<dimension> & p1) C {
			for (size_t i = 0; i < dimension; i++) if (coords[i] > p1[i]) return false;
			return true;
		}
		
		bool operator >  (C Vector<dimension> & p1) C {
			for (size_t i = 0; i < dimension; i++) if (coords[i] <= p1[i]) return false;
			return true;
		}
		
		bool operator <  (C Vector<dimension> & p1) C {
			for (size_t i = 0; i < dimension; i++) if (coords[i] >= p1[i]) return false;
			return true;
		}
		

		double distanceTo(C Vector<dimension> & vector) C {
			double sum = 0;
			for (size_t i = 0; i < dimension; i++)
				sum += (coords[i] - vector.coords[i]) * (coords[i] - vector.coords[i]);
			return sqrt(sum);
		}
		
		double norm() C {
			double sum = 0;
			for (size_t i = 0; i < dimension; i++)
				sum += coords[i] * coords[i];
			return sqrt(sum);
		}
		
		double cosWith(const Vector<dimension> & vec) {
			return (*this) * vec / (norm() * vec.norm());
		}
		
		Vector<dimension> ort() C {
			return (*this) / norm();
		}
		
		Vector<dimension> getAbsVec() C {
			Vector<dimension> out = *this;
			for (size_t i = 0; i < dimension; i++) out[i] = abs(out[i]);
			return out;
		}
		
		Vector<dimension> & truncateTo(double norm_max) {
			double norm = this->norm();
			if (norm > norm_max) *this *= norm_max / norm;
			return *this;
		}
		
		Vector<dimension> & lengthenTo(double norm_min) {
			double norm = this->norm();
			if (norm < norm_min) *this *= norm_min / norm;
			return *this;
		}
		
		void swap(size_t index1, size_t index2) {
			auto buf = coords[index1];
			coords[index1] = coords[index2];
			coords[index2] = buf;
		}
		
		Vector<dimension> & operator = (std::initializer_list<D> list) {
			if (list.size() != dimension) {
				::printf("Error: dimension = %lu, but list dimension = %lu\n", (unsigned long)dimension, (unsigned long)list.size());
				exit(1);
			}
			memcpy(coords, list.begin(), list.size() * sizeof(D));
			return *this;
		}
		
		Vector<dimension> & operator = (C Matrix<dimension, 1> & matrix);
	};
	
	template <size_t dimension>
	Vector<dimension>   operator *  (C D  & d0, C Vector<dimension> & p1) {
		return Vector<dimension>(p1) *= d0;
	}
	

	
}

#undef EPS
#undef C
#undef D

#endif