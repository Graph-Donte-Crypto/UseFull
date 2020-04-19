#ifndef UF_M_Matrix_H
#define UF_M_Matrix_H

#include "Vector.h"
#include "../Utils/Ok.h"

//UseFull Math Matrix module
//Version 1.1 alfa
//Make by Graph Don'te-Crypto

#define C const
#define D double

#define EPS 0.001

namespace math {
	
	
	template <size_t height, size_t width>
	struct Matrix {
		Vector<width> set[height];
		/*
		//Матрица это

		1   2   3
		4   5   6
		7   8   9

		где set[i] -> строка номер i
		где set[i][j] -> элемент в строке i под номером j

		*/
		Matrix() {}
		Matrix(C Matrix & matrix) {
			for (size_t i = 0; i < height; i++) set[i] = matrix[i];
		}
		Matrix(std::initializer_list<Vector<width>> list) {
			
			if (list.size() != height) {
				::printf("Matrix height and list.size() have different values\n", (unsigned long)height, (unsigned long)list.size());
			}
			for (size_t i = 0; i < list.size(); i++) set[i] = *(list.begin() + i);
		}
		void swapRows(size_t line1, size_t line2) {
			auto buf = set[line1];
			set[line1] = set[line2];
			set[line2] = buf;
		}
		void swapColumns(size_t col1, size_t col2) {
			for (size_t i = 0; i < height; i++) set[i].swap(col1, col2);
		}
		void printf(C char * format = "%04.1lf ") {
			for (size_t i = 0; i < height; i++) set[i].printf(format);
			::printf("\n");
		}
		
		Matrix<width, height> transpose() {
			Matrix<width, height> out;
			for (size_t i = 0; i < height; i++)
				for (size_t j = 0; j < width; j++)
					out[j][i] = set[i][j];
			return out;
		}

		Vector<width> & operator[] (size_t i)   {return set[i];}
		Vector<width>   operator[] (size_t i) C {return set[i];}

		Matrix<height, width> & operator = (C Matrix<height, width> & mat) {
			for (size_t i = 0; i < height; i++) set[i]= mat[i];
			return *this;
		}

		template <size_t new_width>
		Matrix<height, new_width>   operator * (C Matrix<width, new_width> & mat) C {
			Matrix<height, new_width> out;
			for (size_t i = 0; i < height; i++) {
				out[i] = 0;
				for (size_t j = 0; j < width; j++) out[i] += set[i][j] * mat[j];
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

		
		Matrix<height, width> & operator *= (C D & a)   { for (size_t i = 0; i < height; i++) set[i] *= a; return *this;}
		Matrix<height, width> & operator /= (C D & a)   { for (size_t i = 0; i < height; i++) set[i] /= a; return *this;}
		Matrix<height, width> & operator -= (C D & a)   { for (size_t i = 0; i < height; i++) set[i] -= a; return *this;}
		Matrix<height, width> & operator += (C D & a)   { for (size_t i = 0; i < height; i++) set[i] += a; return *this;}

		Matrix<height, width> & operator =  (C D & d)   { for (size_t i = 0; i < height; i++) set[i] =  d; return *this;}
		
		Matrix<height, width>   operator *  (C D & a) C { return Matrix<height, width>() *= a;}
		Matrix<height, width>   operator /  (C D & a) C { return Matrix<height, width>() /= a;}
		Matrix<height, width>   operator -  (C D & a) C { return Matrix<height, width>() -= a;}
		Matrix<height, width>   operator +  (C D & a) C { return Matrix<height, width>() += a;}
		
		Matrix<height, 1> & operator = (C Vector<height> & vector) {
			for (size_t i = 0; i < height; i++) set[i][0] = vector[i];
			return *this;
		}
		
		Matrix<height, width> & operator = (std::initializer_list<Vector<width>> list) {	
			if (list.size() != height) {
				::printf("Matrix height and list.size() have different values\n", (unsigned long)height, (unsigned long)list.size());
			}
			for (size_t i = 0; i < list.size(); i++) set[i] = *(list.begin() + i);
			return *this;
		}
		
		Matrix<height, width> & toUpTriangular() {
			if (width != height) {
				printf("width(%ly) != height(%lu) : can't make Upper Triangular Matrix\n");
				exit(1);
			}; 
			for (size_t i = 0; i < width; i++) {
				//if diagonal element ~ 0, try to find non zero
				if (abs(set[i][i]) < EPS) {
					for (size_t j = i + 1; j < height; j++) {
						if (abs(set[j][i]) > EPS) {
							swapRows(i, j);
							break;
						}
					}
				}
				if (abs(set[i][i]) < EPS) continue; 
				for (size_t j = i + 1; j < height; j++)
					set[j] -= set[i] * set[j][i] / set[i][i];
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
		
	};
	
	template <size_t width>
	Vector<width> & Vector<width>::operator = (C Matrix<width, 1> & matrix) {
		for (size_t i = 0; i < width; i++) coords[i] = matrix[i][0];
		return *this;
	}
	
	
	
}

#undef EPS


#undef C
#undef D

#endif
