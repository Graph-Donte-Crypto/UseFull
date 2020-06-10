#ifndef UF_S_Remove_H
#define UF_S_Remove_H

#include "BaseLib.hpp"

//UseFull String Remove module
//Version 1.0 alfa
//Make by Graph Don'te-Crypto

namespace str {

	namespace Remove {

		char * Symbol(char * in, const char symbol) {
			char * ret = in;
			char * buf = in;
			while (true) {
				if (*in != symbol) {
					*buf = *in;
					buf++;
				}
				if (*in == 0) break;
				in++;
			}
			return ret;
		}
		char * Range(char * in, size_t from, size_t to) {
			long long count = strlen(in) - to;
			if (count <= 0) in[from] = 0;
			else for (size_t i = 0; i < (size_t)count; i++)
				in[i + from] = in[i + to + 1];
			return in;
		}
		char * End(char * where, size_t count) {
			long long newend = strlen(where) - count;
			if (newend < 0) newend = 0;
			where[newend] = 0;
			return where;
		}
		char * First(char * where, const char * what) {
			char * ptr = strstr(where, what);
			if (ptr) Remove::Range(ptr, 0, strlen(what));
			return where;
		}

	}

}

#endif