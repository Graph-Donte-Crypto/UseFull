#ifndef UF_S_Level_H
#define UF_S_Level_H

#include "BaseLib.h"

//UseFull String Level module
//Version 1.0 alfa
//Make by Graph Don'te-Crypto

int strLevelDualBefore(const char * where, const char * to, char open, char close, int lvl) {
	while (where < to) {
		if (*where == open) lvl++;
		else if (*where == close) lvl--;
		where++;
	}
	return lvl;
}
int strLevelDual(const char * where, char open, char close, int lvl) {
	while (*where) {
		if (*where == open) lvl++;
		else if (*where == close) lvl--;
		where++;
	}
	return lvl;
}
int strLevelSameBefore(const char * where, const char * to, char same, bool f) {
	while (where < to) {
		if (*where == same) f = !f;
		where++;
	}
	return f;
}
int strLevelSame(const char * where, char same, bool f) {
	while (*where) {
		if (*where == same) f = !f;
		where++;
	}
	return (int)f;
}

#endif
