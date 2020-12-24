#ifndef UF_M_Matrix_H
#define UF_M_Matrix_H

#include <string.h>

#include "Vector.hpp"
#include "../Utils/Ok.hpp"

//UseFull Math Matrix module
//Version 1.1 alfa
//Make by Graph Don'te-Crypto

#define C const
#define D double

#define EPS 0.001

namespace math {
	
	using namespace utils;
	
	template <size_t height, size_t width = height>
	struct Matrix {
		Vector<width> array[height];
		/*
		//Матрица это

		1   2   3
		4   5   6
		7   8   9

		где array[i] -> строка номер i
		где array[i][j] -> элемент в строке i под номером j

		*/
		Matrix() {}
		Matrix(C Matrix & matrix) {
			memcpy(array, matrix.array, width * sizeof(Vector<height>));
		}
		Matrix(std::initializer_list<Vector<width>> list) {
			if (list.size() != height) {
				::printf("Matrix height = (%llu) and list.size() = (%llu) have different values\n", height, list.size());
			}
			memcpy(array, list.begin(), list.size() * sizeof(D));
		}
		
		Matrix<height, width> & set(size_t i, size_t j, double d) {
			array[i][j] = d; return *this;
		}
		
		
		void swapRows(size_t line1, size_t line2) {
			auto buf = array[line1];
			array[line1] = array[line2];
			array[line2] = buf;
		}
		void swapColumns(size_t col1, size_t col2) {
			for (size_t i = 0; i < height; i++) array[i].swap(col1, col2);
		}
		void printf(C char * format = "%04.1lf ") {
			for (size_t i = 0; i < height; i++) array[i].printf(format);
			::printf("\n");
		}
		
		Matrix<width, height> transpose() {
			Matrix<width, height> out;
			for (size_t i = 0; i < height; i++)
				for (size_t j = 0; j < width; j++)
					out[j][i] = array[i][j];
			return out;
		}

		Vector<width> & operator[] (size_t i)   {return array[i];}
		Vector<width>   operator[] (size_t i) C {return array[i];}

		Matrix<height, width> & operator = (C Matrix<height, width> & mat) {
			for (size_t i = 0; i < height; i++) array[i]= mat[i];
			return *this;
		}

		template <size_t new_width>
		Matrix<height, new_width>   operator * (C Matrix<width, new_width> & mat) C {
			Matrix<height, new_width> out;
			for (size_t i = 0; i < height; i++) {
				out[i] = 0;
				for (size_t j = 0; j < width; j++) out[i] += array[i][j] * mat[j];
			}
			return out;
		}
		
		Vector<height> operator * (C Vector<width> & vector) C {
			Vector<height> out;
			for (size_t i = 0; i < height; i++) {
				out[i] = 0;
				for (size_t j = 0; j < width; j++) out[i] += array[i][j] * vector[j];
			}
			return out;
		}
		
		Matrix<height, width>   operator + (C Matrix<height, width> & mat) C {
			Matrix out(*this);
			for (size_t i = 0; i < height; i++) out[i] += mat[i];
			return out;
		}
		Matrix<height, width>   operator - (C Matrix<height, width> & mat) C {
			Matrix out(*this);
			for (size_t i = 0; i < height; i++) out[i] -= mat[i];
			return out;
		}

		
		Matrix<height, width> & operator *= (C D & a)   { for (size_t i = 0; i < height; i++) array[i] *= a; return *this;}
		Matrix<height, width> & operator /= (C D & a)   { for (size_t i = 0; i < height; i++) array[i] /= a; return *this;}
		Matrix<height, width> & operator -= (C D & a)   { for (size_t i = 0; i < height; i++) array[i] -= a; return *this;}
		Matrix<height, width> & operator += (C D & a)   { for (size_t i = 0; i < height; i++) array[i] += a; return *this;}

		Matrix<height, width> & operator =  (C D & d)   { for (size_t i = 0; i < height; i++) array[i] =  d; return *this;}
		
		Matrix<height, width>   operator *  (C D & a) C { return Matrix<height, width>() *= a;}
		Matrix<height, width>   operator /  (C D & a) C { return Matrix<height, width>() /= a;}
		Matrix<height, width>   operator -  (C D & a) C { return Matrix<height, width>() -= a;}
		Matrix<height, width>   operator +  (C D & a) C { return Matrix<height, width>() += a;}
		
		Matrix<height, 1> & operator = (C Vector<height> & vector) {
			for (size_t i = 0; i < height; i++) array[i][0] = vector[i];
			return *this;
		}
		
		Matrix<height, width> & operator = (std::initializer_list<Vector<width>> list) {	
			if (list.size() != height) {
				::printf("Matrix height and list.size() have different values\n", (unsigned long)height, (unsigned long)list.size());
			}
			for (size_t i = 0; i < list.size(); i++) array[i] = *(list.begin() + i);
			return *this;
		}
		
		Matrix<height, width> & toUpTriangular() {
			if (width != height) {
				printf("width(%ly) != height(%lu) : can't make Upper Triangular Matrix\n");
				exit(1);
			}; 
			for (size_t i = 0; i < width; i++) {
				//if diagonal element ~ 0, try to find non zero
				if (abs(array[i][i]) < EPS) {
					for (size_t j = i + 1; j < height; j++) {
						if (abs(array[j][i]) > EPS) {
							swapRows(i, j);
							break;
						}
					}
				}
				if (abs(array[i][i]) < EPS) continue; 
				for (size_t j = i + 1; j < height; j++)
					array[j] -= array[i] * array[j][i] / array[i][i];
			}
			return *this;
		}
		
		Ok<Matrix<height, width>> getUpTriangular() C {
			if (width != height) return {};
			else return Matrix(*this).toUpTriangular();
		}
		
		Ok<D> determinant() C {
			if (width != height) return {};
			Matrix<height, width> triangle = getUpTriangular();
			double ans = 1;
			for (size_t i = 0; i < height; i++) ans *= triangle[i][i];
			return ans;
		}
		
		Matrix<height> & toE() {
			for (size_t i = 0; i < height; i++) array[i] = 0;
			for (size_t i = 0; i < height; i++) array[i][i] = 1;
			return *this;
		}
		
		
		static Matrix<height> E() {
			return Matrix<height>().toE();
		}
		
		Matrix<height> & setRotateAngle(size_t i, size_t j, double angle) {
			return set(i, i,  cos(angle))
				  .set(i, j, -sin(angle))
				  .set(j, i,  sin(angle))
				  .set(j, j,  cos(angle));
		}
		
		static Matrix<height> Rotate(size_t i, size_t j, double angle) {
			return Matrix<height>::E().setRotateAngle(i, j, angle);
		}
	};
	
	template <size_t width>
	Vector<width> & Vector<width>::operator = (C Matrix<width, 1> & matrix) {
		for (size_t i = 0; i < width; i++) coords[i] = matrix[i][0];
		return *this;
	}
	
	Matrix<3> Matrix3RotateFromVectorAndAngle(C Vector<3> & v, double a) {
		const double ca = cos(a);
		const double ca1 = 1 - ca;
		const double sa = sin(a);
		const double & x = v[0];
		const double & y = v[1];
		const double & z = v[2];
		return Matrix<3>({
			{ca + ca1 * x * x, ca1 * x * y - sa * z, ca1 * x * y + sa * y},
			{ca1 * y * x + sa * z, ca + ca1 * y * y, ca1 * y * z - sa * x},
			{ca1 * z * x - sa * y, ca1 * z * y + sa * x, ca + ca1 * z * z}
		});
	}
	
	
	
}

#undef EPS


#undef C
#undef D

#endif
