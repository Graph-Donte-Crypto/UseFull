#ifndef UF_S_Replace_H
#define UF_S_Replace_H

#include "BaseLib.h"

//UseFull String Replace module
//Version 1.0 alfa
//Make by Graph Don'te-Crypto

char * strReplaceStandart(char * where, const char * what, const char * with_what) {
	char * pos = where;
	size_t with_what_len = strlen(with_what);
	size_t what_len = strlen(what);
	long long shift = with_what_len - what_len;
	if (shift) {
		char * bufptr = NULL;
		while (( pos = strstr(pos, what) ) != NULL) {
			bufptr = pos + what_len;
			memmove(bufptr + shift, bufptr, strlen(bufptr) + 1);
			strncpy(pos, with_what, with_what_len);
			pos += with_what_len;
		}
	}
	else {
		while (( pos = strstr(pos, what) ) != NULL) {
			strncpy(pos, with_what, with_what_len);
			pos += with_what_len;
		}
	}
	return where;
}
char * strReplaceFirst(char * where, const char * what, const char * with_what) {
	char * pos = strstr(where, what);
	if (pos) {
		size_t with_what_len = strlen(with_what);
		size_t what_len = strlen(what);
		if (with_what_len - what_len)
			memmove(pos + what_len + with_what_len - what_len, pos + what_len, strlen(pos + what_len));
		strncpy(pos, with_what, with_what_len);
	}
	return where;
}
char * strReplaceRepeat(char * where, const char * what, const char * with_what) {
	char * pos = where;
	while (( pos = strstr(pos, what) ) != NULL) {
		strReplaceStandart(pos, what, with_what);
	}
	return where;
}
char * strReplaceRaw(char * where, int pos, const char * with_what, int shift) {
	if (shift < 0) {
		int pk = pos - shift;
		int count = strlen(where) - pos;
		for (int i = count; i >= 0; i--)
			where[i + pk] = where[i + pos];
	}
	else if (shift > 0) {
		int pk = pos + shift;
		int count = - pos - shift + 1 + strlen(where);
		for (int i = 0; i < count; i++)
			where[i + pos] = where[i + pk];
	}
	strncpy(where + pos, with_what, strlen(with_what));
	return where;
}

#endif
