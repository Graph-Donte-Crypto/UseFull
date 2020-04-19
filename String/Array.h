#ifndef UF_S_Array_H
#define UF_S_Array_H

#include "BaseLib.h"

//UseFull String Array module
//Version 1.0 alfa
//Make by Graph Don'te-Crypto

namespace str {
	
	namespace Array {

		void Copy(char ** out, const char ** in, size_t n) {
			for (size_t i = 0; i < n ; i++) strcpy(out[i], in[i]);
		}
		void CopyRange(char ** out, const char ** in, size_t n, size_t from = 0, long long to = -1) {
			if (to == -1) for (size_t i = from; i < n ; i++) strcpy(out[i     ], in[i]);
			else          for (size_t i = from; i < (size_t)to; i++) strcpy(out[i-from], in[i]);
		}

		void Print(const char ** in, size_t n) {
			for (size_t i = 0; i < n; i++) printf("%03lu|%s\n", (unsigned long)i, in[i]);
		}
		void PrintPair(const char ** in1 , const char ** in2, size_t n) {
			for (size_t i = 0; i < n; i++) printf("%03lu|%s|%s\n", (unsigned long)i, in1[i], in2[i]);
		}
		void Print(char ** in, size_t n) {
			for (size_t i = 0; i < n; i++) printf("%03lu|%s\n", (unsigned long)i, in[i]);
		}
		void PrintPair(char ** in1 , char ** in2, size_t n) {
			for (size_t i = 0; i < n; i++) printf("%03lu|%s|%s\n", (unsigned long)i, in1[i], in2[i]);
		}

		char ** Concat(char ** out, const char ** in, long long n = -1) {
			char ** ret = out;
			while (**out != 0) out++; //find end of 'out'
			if (n == -1) {
				while (**in != 0) {
					strcpy(*out, *in);
					out++;
					in++;
				}
			}
			else {
				while (n != 0 && **in != 0) {
					n--;
					strcpy(*out, *in);
					out++;
					in++;
				}
			}
			return ret;
		}
		size_t Length(const char ** in, long long limit = -1) {
			size_t count = 0;
			if (limit == -1) while (in[count][0]) {
				count++;
			}
			else while (limit != 0) {
				limit--;
				if (in[count][0] == 0) break;
				count++;
			}
			return count;
		}
		size_t Length(char ** in, long long limit = -1) {
			return Array::Length((const char **)in, limit);
		}
		
		char ** RemoveEmpty(char ** in, size_t limit) {
			size_t shift = 0;
			for (size_t i = 0; i < limit; i++) {
				if (shift != 0) {
					strcpy(in[i - shift], in[i]);
				}
				if (*in[i] == 0) shift += 1;
			}
			for (size_t i = limit - shift; i < limit; i++) in[i][0] = 0;
			return in;
		}
		//TODO: need tests
		char ** RemoveEmpty2(char ** in, size_t limit) {
			size_t shift = 0;
			char * swapptr = nullptr;
			for (size_t i = 0; i < limit; i++) {
				if (*in[i] == 0) shift += 1;
				else if (shift) {
					swapptr = in[i - shift];
					in[i - shift] = in[i];
					in[i] = swapptr;
				}
			}
			return in;
		}
		char ** Destroy(char ** in, int n) {
			for (int i = 0; i < n; i++) delete[] in[i];
			delete[] in;
			return nullptr;
		}
		char ** Make(int n, int m = 256) {
			char ** buf = new char *[n];
			for (int i = 0; i < n; i++) {
				buf[i] = new char[m];
				buf[i][0] = 0;
			}
			return buf;
		}

	}
}
#endif