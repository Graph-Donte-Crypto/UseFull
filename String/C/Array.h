#ifndef UF_S_Array_H
#define UF_S_Array_H

#include "BaseLib.h"

//UseFull String Array module
//Version 1.0 alfa
//Make by Graph Don'te-Crypto

void strArrayCopy(char ** out, const char ** in, size_t n) {
	for (size_t i = 0; i < n ; i++) strcpy(out[i], in[i]);
}
void strArrayCopyFrom(char ** out, const char ** in, size_t n, size_t from) {
	for (size_t i = from; i < n ; i++) strcpy(out[i], in[i]);
}

void strArrayCopyFromTo(char ** out, const char ** in, size_t from, size_t to) {
	for (size_t i = from; i < (size_t)to; i++) strcpy(out[i-from], in[i]);
}

void strArrayPrint(const char ** in, size_t n) {
	for (size_t i = 0; i < n; i++) printf("%03lu|%s\n", (unsigned long)i, in[i]);
}
void strArrayPrintPair(const char ** in1 , const char ** in2, size_t n) {
	for (size_t i = 0; i < n; i++) printf("%03lu|%s|%s\n", (unsigned long)i, in1[i], in2[i]);
}

char ** strArrayConcatFull(char ** out, const char ** in) {
	char ** ret = out;
	while (**out != 0) out++; //find end of 'out'
	while (**in != 0) {
		strcpy(*out, *in);
		out++;
		in++;
	}
	return ret;
}

char ** strArrayConcatAmount(char ** out, const char ** in, size_t n) {
	char ** ret = out;
	while (**out != 0) out++; //find end of 'out'
	while (n != 0 && **in != 0) {
		n--;
		strcpy(*out, *in);
		out++;
		in++;
	}
	return ret;
}

size_t strArrayLength(const char ** in, size_t limit) {
	size_t count = 0;
	while (limit != 0) {
		limit--;
		if (in[count][0] == 0) break;
		count++;
	}
	return count;
}

char ** strArrayRemoveEmpty(char ** in, size_t limit) {
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
char ** strArrayRemoveEmpty2(char ** in, size_t limit) {
	size_t shift = 0;
	char * swapptr = NULL;
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
char ** strArrayDestroy(char ** in, int n) {
	for (int i = 0; i < n; i++) strDestroy(&in[i]);
	free(in);
	return NULL;
}
char ** strArrayMake(int n, int m) {
	char ** buf = (char **)malloc(sizeof(char *) * n);
	for (int i = 0; i < n; i++)
		buf[i] = strInit(m);
	return buf;
}

#endif
