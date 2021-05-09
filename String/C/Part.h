#ifndef UF_S_Part_H
#define UF_S_Part_H

#include "BaseLib.h"
//UseFull String Part module 
//Version 1.0 alfa
//Make by Graph Don'te-Crypto


//TODO: модуль Part очень сложен для понимания
//Для каждого метода нужно сделать пояснение и привести примеры использовани
//Возвращает указатель на первый символ после splitter, если таковых нет - возвращает NULL
char * strPartNextSkip(char * in, char splitter) {
	while (*in != splitter) {
		if (*in == 0) return NULL;
		in++;
	}
	return *in ? in + 1 : in;
}

char * strPartNextSkipLevelSame(char * in, char splitter, char target) {
	if (*in == 0) return NULL;
	bool f = true;
	while (true) {
		if (*in == target) f = !f;
		if ( (*in == splitter && f) || *in == 0) break;
		in++;
	}
	return *in ? in + 1 : in;
}
//Возвращает указатель на первый символ после splitter, если in[0] == 0 возвращает NULL
//Строку между in и первым символом splitter пишет в out
char * strPartNextGet(char * in, char * out, char splitter) {
	if (*in == 0) return NULL;
	while (true) {
		*out = *in;
		if (*out == splitter || *out == 0) break;
		out++;
		in++;
	}
	*out = 0;
	return *in ? in + 1 : in;
}
//Возвращает указатель на первый символ после splitter, если in[0] == 0 возвращает NULL
//Строку между in и первым символом splitter пишет в out, предварительно выделев под него память
char * strPartNextGetNew(char * in, char ** out, char splitter) {
	if (*in == 0) return NULL;
	char * inptr = in;
	size_t count = 0;
	while (true) {
		count++;
		if (*in == splitter || *in == 0) break;
		in++;
	}

	*out = strInit(count);
	strncpy(*out, inptr, count-1);
	*out[count-1] = 0;
	return *in ? in+1 : in;
}
/*
Возвращает указатель на первый символ после splitter, если in[0] == 0 возвращает NULL
Строку между in и первым символом splitter пишет в out, предварительно выделев под него память
при нахождение символа open lvl увеличивается на 1, при нахождение close - уменьшается на 1
splitter учитывается только если lvl == 0
Примеры кода:
GetLevelDual("Hello (my dear(world)) afterwhat", str, ' ', '(', ')', 0) -> str == Hello;
GetLevelDual("(my dear(world)) afterwhat", str, ' ', '(', ')', 0) -> str = (my dear(world))
GetLevelDual("afterwhat", str, ' ', '(', ')', 0) -> str = afterwhat
*/

char * strPartNextGetLevelDual(char * in, char * out, char splitter, char open, char close, int lvl) {
	if (*in == 0) return NULL;
	while (true) {
		*out = *in;
		if (*out == open) lvl++;
		else if (*out == close) lvl--;
		if ( (*out == splitter && lvl == 0) || *out == 0) break;
		out++;
		in++;
	}
	*out = 0;
	return *in ? in + 1 : in;
}
char * strPartNextGetLevelSame(char * in, char * out, char splitter, char target) {
	if (*in == 0) return NULL;
	bool f = true;
	while (true) {
		*out = *in;
		if (*out == target) f = !f;
		if ( (*out == splitter && f) || *out == 0) break;
		out++;
		in++;
	}
	*out = 0;
	return *in ? in + 1 : in;
}
//TODO: remake PREV: make return like NEXT & make ..Level && ..LevelSame functions
char * strPartPrevSkipSame(char * str_start, char * in, char target) {
	while (true) {
		if (in == str_start) return NULL;
		if (*in == target) break;
		in--;
	}
	return in-1;
}
char * strPartPrevGet(char * str_start, char * in, char * out, char splitter) {
	char * buf_in = in;
	char * ret = buf_in;
	while (true) {
		if (*buf_in == splitter) break;
		if (buf_in == str_start) return NULL;
		buf_in--;
	}
	ret = buf_in - 1;
	buf_in++;
	while (true) {
		*out = *buf_in;
		out++;
		if (buf_in == in) break;
		buf_in++;
	}
	*out = 0;
	return ret;
}
#endif
