#ifndef UF_S_Level_H
#define UF_S_Level_H

#include "BaseLib.h"

//UseFull String Level module
//Version 1.0 alfa
//Make by Graph Don'te-Crypto

namespace str {
	
	namespace Level {

		int Dual(const char * where, const char * to, char open, char close, int lvl = 0) {
			while (where < to) {
				if (*where == open) lvl++;
				else if (*where == close) lvl--;
				where++;
			}
			return lvl;
		}
		int Dual(const char * where, char open, char close, int lvl = 0) {
			while (*where) {
				if (*where == open) lvl++;
				else if (*where == close) lvl--;
				where++;
			}
			return lvl;
		}
		int Same(const char * where, const char * to, char same, bool f = false) {
			while (where < to) {
				if (*where == same) f = !f;
				where++;
			}
			return f;
		}
		int Same(const char * where, char same, bool f = false) {
			while (*where) {
				if (*where == same) f = !f;
				where++;
			}
			return (int)f;
		}

	}

}

#endif