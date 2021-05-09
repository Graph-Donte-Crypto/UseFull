#ifndef UF_S_Compare_H
#define UF_S_Compare_H

#include "BaseLib.h"

//UseFull String Compare module
//Version 1.0 alfa
//Make by Graph Don'te-Crypto

bool strCompareWeak(const char * in1, const char * in2) {
	while (true) {
		if (*in1 != *in2) return false;
		if (*in1 == 0)	  return true;
		in1++;
		in2++;
	}
}
bool strCompareWeakUncase(const char * in1, const char * in2) {
	while (true) {
		if (caseLower[(unsigned char)(*in1)] != caseLower[(unsigned char)(*in2)]) return false;
		if (*in1 == 0) return true;
		in1++;
		in2++;
	}
}
int strCompareStrong(const char * in1, const char * in2) {
	while (true) {
		if (*in1 != *in2) {
			if (*in1 > *in2) return 1;
			else			 return -1;
		}
		if (*in1 == 0)	     return 0;
		in1++;
		in2++;
	}
}

int strCompareStrongWithAlphabet(const char * in1, const char * in2, const char * alphabet) {
	long long pos1, pos2;
	const char * where_ptr;
	while (true) {
		if (*in1 == 0) {
			if (*in2 == 0)  return 0;
			else			return -1;
		}
		else if (*in2 == 0) return 1;

		pos1 = -1;
		where_ptr = alphabet;
		while (*where_ptr != 0) {
			if (*where_ptr == *in1) {
				pos1 = where_ptr - alphabet;
				break;
			}
			where_ptr++;
		}

		pos2 = -1;
		where_ptr = alphabet;
		while (*where_ptr != 0) {
			if (*where_ptr == *in2) {
				pos2 = where_ptr - alphabet;
				break;
			}
			where_ptr++;
		}

		if (pos1 != pos2) {
			if (pos1 > pos2)   return 1;
			else		       return -1;
		}
		in1++;
		in2++;
	}
}


#endif
