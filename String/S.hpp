#ifndef UF_S_S_H
#define UF_S_S_H

#include <stdlib.h>
#include <initializer_list>

#include "BaseLib.hpp"

//UseFull String Variable length module
//Version 1.0 alfa
//Make by Graph Don'te-Crypto

namespace str {
	struct SorCPtr;

	struct S {
		static const size_t defsize = 1;
		char * ptr = nullptr;
		size_t maxsize = defsize;
		size_t length = 0;
		bool autoExpand = true;


		size_t _calcLen() {
			return length = strlen(ptr);
		}
		size_t _countBytes(bool trust = true) {
			if (trust) return length + 1;
			else return _calcLen() + 1;
		}
		void _printInfo() {
			printf("ptr(%lu) -> %s\n    maxsize = %lu\n    length = %lu\n    autoExpand = %lu\n", (long unsigned)((size_t)ptr), ptr, (long unsigned)maxsize, (long unsigned)length, (long unsigned)autoExpand);
		}
		S & _checkSize(size_t newlen) {
			if (newlen >= maxsize) {
				if (autoExpand) {
					do {
						maxsize*=2;
					} while (newlen > maxsize);

					char * buf = Init(maxsize);
					memcpy(buf, ptr, _countBytes());
					Destroy(ptr);
					ptr = buf;
				}
				else {
					printf("str::S ExpandError\nautoExpand == false\nCall from str::S::_checkSize\n");
					_printInfo();
					exit(1);
				}
			}
			return *this;
		}
		void _checkSizeNoCopy(size_t newlen) {
			if (newlen > maxsize) {
				if (autoExpand) {
					do {
						maxsize*=2;
					} while (newlen > maxsize);
					Destroy(ptr);
					Init(ptr, maxsize);
				}
				else {
					printf("str::S ExpandError\nautoExpand == false\nCall from str::S::_checkSizeNoCopy\n");
					_printInfo();
					exit(1);
				}
			}
		}

		S & set(const char * s, size_t len) {
			_checkSizeNoCopy(len);
            CopyN(ptr, s, len);
            ptr[len] = 0;
			length = len;
            return *this;
        }

		S() {
			Init(ptr, maxsize);
		}
		S(size_t size) {
			length = size;
			while (length > maxsize) maxsize *= 2;
			Init(ptr, maxsize);
		}
		S(const char * s) {
			length = strlen(s);
			while (length > maxsize) maxsize *= 2;
			Init(ptr, maxsize);
			CopyN(ptr, s, length + 1);
		}
		S(char * s) {
			length = strlen(s);
			while (length > maxsize) maxsize *= 2;
			Init(ptr, maxsize);
			CopyN(ptr, s, length + 1);
		}
		S(const S & s) {
			length  = s.length;
			maxsize = s.maxsize;
			Init(ptr, maxsize);
			CopyN(ptr, s.ptr, length + 1);
			autoExpand = s.autoExpand;
		}
		S(const S * s) {
			length  = s->length;
			maxsize = s->maxsize;
			Init(ptr, maxsize);
			CopyN(ptr, s->ptr, length + 1);
			autoExpand = s->autoExpand;
		}
		S(std::initializer_list<char> list) {
			length = list.size();
			while (length > maxsize) maxsize *= 2;
			Init(ptr, maxsize);
			CopyN(ptr, list.begin(), length);
		}

		~S() {
			Destroy(ptr);
		}
		S   copy() {
			return S(this);
		}

		S & insert(SorCPtr socp, size_t pos);
		//S & append(SorCPtr socp);
		S & replace(SorCPtr what_s, SorCPtr with_what_s);
		S & replaceRepeat(SorCPtr what_s, SorCPtr with_what_s);
		S & operator += (SorCPtr socp);
		S & operator << (SorCPtr socp);

		S & append(const char * s, size_t len) {
			_checkSize(length + len + 1);
			CopyN(ptr + length, s, len + 1);
			length += len;
			return *this;
		}
		S & append(const char   s) {
			_checkSize(length + 2);
			ptr[length]     = s;
			ptr[length + 1] = 0;
			length += 1;
			return *this;
		}
		S & append(const char * s) {return append( s, strlen(s));}
		S & append(S * s)          {return append(*s, s->length);}
		S & append(S & s)          {return append( s, s. length);}

		S & removeSymbol(const char symbol) {
			char * buf = ptr;
			char * in = ptr;
			while (true) {
				if (*in != symbol) {
					*buf = *in;
					buf++;
				}
				else length--;
				if (*in == 0) break;
				in++;
			}
			return *this;
		}
		S & removeRange(size_t from, size_t to) {
			long long count = length - to;
			if (count <= 0) {
				ptr[from] = 0;
				length = from;
			}
			else {
				Move(ptr + from, ptr + to + 1, (size_t)count);
				length -= to + 1 - from;
			}
			return *this;
		}
		S & removeEnd(size_t count) {
			long long tmplen = length - count;
			if (tmplen < 0) length = 0;
			else length = tmplen;
			ptr[length] = 0;
			return *this;
		}

		S & reset() {
			ptr[0] = 0;
			length = 0;
			return *this;
		}

		char * last() {
			return ptr + length - 1;
		}

		char & operator [] (size_t argc) {
			return ptr[argc];
		}

		S & operator = (const char c) {
			ptr[0] = (char)c;
			ptr[1] = 0;
			length = 1;
			return *this;
		}
		S & operator = (const char * s) {
			length = strlen(s);
			_checkSizeNoCopy(length);
			CopyN(ptr, s, length + 1);
			return *this;
		}
		S & operator = (char * s) {
			length = strlen(s);
			_checkSizeNoCopy(length);
			CopyN(ptr, s, length + 1);
			return *this;
		}
		S & operator = (S & s) {
			length  = s.length;
			maxsize = s.maxsize;
			str::Destroy(ptr);
			str::Init(ptr, maxsize);
			CopyN(ptr, s.ptr, s._countBytes());
			autoExpand = s.autoExpand;
			return *this;
		}
		S & operator = (std::initializer_list<char> list) {
			length = list.size();
			while (length > maxsize) maxsize *= 2;
			_checkSizeNoCopy(length);
			CopyN(ptr, list.begin(), length);
			return *this;
		}

		operator char * () {
			return ptr;
		}
		operator const char * () {
			return (const char *)ptr;
		}
	};

	struct SorCPtr {
		S temp;
		S * temp_normal = nullptr;

		SorCPtr(char * s)       {temp = s;}
		SorCPtr(const char   s) {temp = s;}
		SorCPtr(const char * s) {temp = s;}
		SorCPtr(std::initializer_list<char> s) {temp = s;}
		SorCPtr(S & s) {temp_normal = &s;}
		SorCPtr(S * s) {temp_normal =  s;}

		SorCPtr(SorCPtr * s) {
			if (s->temp_normal == nullptr) temp_normal = &(s->temp);
			else temp_normal = s->temp_normal;
		}

		operator char * () {
			if (temp_normal) return temp_normal->ptr;
			else return temp.ptr;
		}
		operator const char * () {
			if (temp_normal) return (const char *)temp_normal->ptr;
			else return (const char *)temp.ptr;
		}
		operator S & () {
			if (temp_normal) return *temp_normal;
			else return temp;
		}
	};

	S & S::insert(SorCPtr socp, size_t pos) {
		S & s = socp;
		_checkSize(length + s.length);
		Move(ptr + pos + s.length, ptr + pos, length - pos + 1);
		CopyN(ptr + pos, s, s.length);
		length += s.length;
		return *this;
	}
	S & S::operator << (SorCPtr socp) {
		S & s = socp;
		_checkSize(length + s.length + 1);
		CopyN(ptr + length, s.ptr, s.length + 1);
		length += s.length;
		return *this;
	}

	S & S::replace(SorCPtr what_s, SorCPtr with_what_s) {
		S & what = what_s;
		S & with_what = with_what_s;
		char * pos = ptr;
		long long shift = with_what.length - what.length;
		char * bufptr = nullptr;
		if (shift < 0) {
			while (( pos = strstr(pos, what) ) != nullptr) {
				_checkSize(length + shift);
				bufptr = pos + what.length;
				str::Move(bufptr + shift, bufptr, length + ptr - bufptr + 1); // length + ptr - bufptr == strlen(bufptr)
				strncpy(pos, with_what, with_what.length);
				pos += with_what.length;
				length += shift;
			}
		}
		else if (shift > 0) {
			size_t d_pos = pos - ptr;
			while (( pos = strstr(pos, what) ) != nullptr) {
				d_pos    = pos    - ptr;
				_checkSize(length + shift);
				pos      = ptr    + d_pos;
				bufptr = pos + what.length;
				str::Move(bufptr + shift, bufptr, length + ptr - bufptr + 1); // length + ptr - bufptr == strlen(bufptr)
				strncpy(pos, with_what, with_what.length);
				pos += with_what.length;
				length += shift;
			}
		}
		else {
			while (( pos = strstr(pos, what) ) != nullptr) {
				strncpy(pos, with_what, with_what.length);
				pos += with_what.length;
			}
		}
		return *this;
	}
	S & S::replaceRepeat(SorCPtr what_s, SorCPtr with_what_s) {
		S & what = what_s;
		S & with_what = with_what_s;
		char * pos = ptr;
		char * posbuf = nullptr;
		long long shift = with_what.length - what.length;
		char * bufptr = nullptr;

		if (shift < 0) {
			while (( pos = strstr(pos, what) ) != nullptr) {
				posbuf = pos;
				do {
					bufptr = posbuf + what.length;
					memmove(bufptr + shift, bufptr, strlen(bufptr) + 1);
					strncpy(posbuf, with_what, with_what.length);
					posbuf += with_what.length;
					length += shift;
				} while (( posbuf = strstr(posbuf, what) ) != nullptr);
			}
		}
		else if (shift > 0) {
			size_t d_pos = pos - ptr;
			size_t d_posbuf = posbuf - ptr;
			while (( pos = strstr(pos, what) ) != nullptr) {
				posbuf = pos;
				do {
					d_pos    = pos    - ptr;
					d_posbuf = posbuf - ptr;
					_checkSize(length + shift);
					pos      = ptr    + d_pos;
					posbuf   = ptr    + d_posbuf;

					bufptr = posbuf + what.length;
					memmove(bufptr + shift, bufptr, strlen(bufptr) + 1);
					strncpy(posbuf, with_what, with_what.length);
					posbuf += with_what.length;
					length += shift;
				} while (( posbuf = strstr(posbuf, what) ) != nullptr);
			}
		}
		else {
			while (( pos = strstr(pos, what) ) != nullptr) {
				posbuf = pos;
				do {
					strncpy(posbuf, with_what, with_what.length);
					posbuf += with_what.length;
				}   while (( posbuf = strstr(posbuf, what) ) != nullptr);
			}
		}
		return *this;
	}

	S & CopyRange(SorCPtr out, SorCPtr in, size_t from = 0, long long to = -1) {
		S & sout = out;
		S & sin = in;
		if (to == -1) {
			sout = sin.ptr + from;
		}
		else {
			CopyN(sout.ptr, sin.ptr + from, to - from);
			sout[to - from] = 0;
			sout.length = to - from;
		};
		return sout;
	}

}

#endif