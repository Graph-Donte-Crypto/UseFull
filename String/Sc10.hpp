#ifndef UF_S_Sc10_H
#define UF_S_Sc10_H

#include <initializer_list>

#include "BaseLib.hpp"

//UseFull String Constant length(1024 bytes) module
//Version 1.1 release
//Make by Graph Don'te-Crypto

namespace str {

    struct Sc10 {
        char ptr[1024];
        size_t length = 0;
        size_t _calcLen() {
            return length = strlen(ptr);
        }
        size_t _countBytes(bool trust = true) {
            if (trust) return length + 1;
            else return _calcLen() + 1;
        }
        void _printInfo() {
            printf("ptr(%lu) -> %s\n    length = %lu\n", (long unsigned)((size_t)ptr), ptr, (long unsigned)length);
        }

        Sc10 & set(const char * s, size_t len) {
            CopyN(ptr, s, len);
            ptr[len] = 0;
			length = len;
            return *this;
        }

        Sc10() {ptr[0] = 0;}
        Sc10(const char * s,
            size_t s_length) {set( s, s_length );}
        Sc10(const char * s) {set( s, strlen(s));}
        Sc10(      char * s) {set( s, strlen(s));}
        Sc10(const Sc10 & s) {set( s, s. length);}
        Sc10(const Sc10 * s) {set(*s, s->length);}
        Sc10(std::initializer_list<char> list) {
            length = list.size();
            CopyN(ptr, list.begin(), length);
        }

        ~Sc10() {}
        Sc10 copy() {return Sc10(this);}

        Sc10 & insert(const char * s, size_t len, size_t pos) {
            Move(ptr + pos + len, ptr + pos, length - pos + 1);
            CopyN(ptr + pos, s, len);
            length += len;
            return *this;
        }
        Sc10 & insert(const char   s, size_t pos) {return insert(&s, 1        , pos);}
        Sc10 & insert(      char * s, size_t pos) {return insert( s, strlen(s), pos);}
        Sc10 & insert(      Sc10 * s, size_t pos) {return insert(*s, s->length, pos);}
        Sc10 & insert(      Sc10 & s, size_t pos) {return insert( s, s. length, pos);}

        Sc10 & append(const char * s, size_t len) {
            CopyN(ptr + length, s, len);
            length += len;
            ptr[length] = 0;
            return *this;
        }
        Sc10 & append(const char   s) {return append(&s, 1);}
        Sc10 & append(const char * s) {return append( s, strlen(s));}
        Sc10 & append(      char * s) {return append( s, strlen(s));}
        Sc10 & append(      Sc10 * s) {return append(*s, s->length);}
        Sc10 & append(      Sc10 & s) {return append( s, s. length);}

        Sc10 & operator += (      char * s) {return append(s);}
        Sc10 & operator += (const char * s) {return append(s);}
        Sc10 & operator += (      char   s) {return append(s);}
        Sc10 & operator += (      Sc10 & s) {return append(s);}
        Sc10 & operator += (      Sc10 * s) {return append(s);}

        Sc10 & operator << (      char * s) {return append(s);}
        Sc10 & operator << (const char * s) {return append(s);}
        Sc10 & operator << (      char   s) {return append(s);}
        Sc10 & operator << (      Sc10 & s) {return append(s);}
        Sc10 & operator << (      Sc10 * s) {return append(s);}

        Sc10 & replace(const char * what, size_t what_l, const char * with, size_t with_l) {
            char * pos = ptr;
            long long shift = with_l - what_l;
            if (shift != 0 ) {
                char * bufptr = nullptr;
                while (( pos = strstr(pos, what) ) != nullptr) {
                    bufptr = pos + what_l;
                    str::Move(bufptr + shift, bufptr, length + ptr - bufptr + 1); // length + ptr - bufptr == strlen(bufptr)
                    strncpy(pos, with, with_l);
                    pos += with_l;
                    length += shift;
                }
            }
            else {
                while (( pos = strstr(pos, what) ) != nullptr) {
                    strncpy(pos, with, with_l);
                    pos += with_l;
                }
            }
            return *this;
        }

        Sc10 & replace(const char   s, const char   c) {return replace(&s, 1        ,&c, 1        );}
        Sc10 & replace(const char   s,       char * c) {return replace(&s, 1        , c, strlen(c));}
        Sc10 & replace(const char   s,       Sc10 * c) {return replace(&s, 1        ,*c, c->length);}
        Sc10 & replace(const char   s,       Sc10 & c) {return replace(&s, 1        , c, c. length);}
        Sc10 & replace(      char * s, const char   c) {return replace( s, strlen(s),&c, 1        );}
        Sc10 & replace(      char * s,       char * c) {return replace( s, strlen(s), c, strlen(c));}
        Sc10 & replace(      char * s,       Sc10 * c) {return replace( s, strlen(s),*c, c->length);}
        Sc10 & replace(      char * s,       Sc10 & c) {return replace( s, strlen(s), c, c. length);}
        Sc10 & replace(      Sc10 * s, const char   c) {return replace(*s, s->length,&c, 1        );}
        Sc10 & replace(      Sc10 * s,       char * c) {return replace(*s, s->length, c, strlen(c));}
        Sc10 & replace(      Sc10 * s,       Sc10 * c) {return replace(*s, s->length,*c, c->length);}
        Sc10 & replace(      Sc10 * s,       Sc10 & c) {return replace(*s, s->length, c, c. length);}
        Sc10 & replace(      Sc10 & s, const char   c) {return replace( s, s. length,&c, 1        );}
        Sc10 & replace(      Sc10 & s,       char * c) {return replace( s, s. length, c, strlen(c));}
        Sc10 & replace(      Sc10 & s,       Sc10 * c) {return replace( s, s. length,*c, c->length);}
        Sc10 & replace(      Sc10 & s,       Sc10 & c) {return replace( s, s. length, c, c. length);}

        Sc10 & replaceRepeat(const char * what, size_t what_l, const char * with, size_t with_l) {
            char * pos = ptr;
            char * posbuf = nullptr;
            long long shift = with_l - what_l;
            char * bufptr = nullptr;

            if (shift != 0) {
                while (( pos = strstr(pos, what) ) != nullptr) {
                    posbuf = pos;
                    do {
                        bufptr = posbuf + what_l;
                        memmove(bufptr + shift, bufptr, strlen(bufptr) + 1);
                        strncpy(posbuf, with, with_l);
                        posbuf += with_l;
                        length += shift;
                    } while (( posbuf = strstr(posbuf, what) ) != nullptr);
                }
            }
            else {
                while (( pos = strstr(pos, what) ) != nullptr) {
                    posbuf = pos;
                    do {
                        strncpy(posbuf, with, with_l);
                        posbuf += with_l;
                    }	while (( posbuf = strstr(posbuf, what) ) != nullptr);
                }
            }
            return *this;
        }

        Sc10 & replaceRepeat(const char   s, const char   c) {return replaceRepeat(&s, 1        ,&c, 1        );}
        Sc10 & replaceRepeat(const char   s,       char * c) {return replaceRepeat(&s, 1        , c, strlen(c));}
        Sc10 & replaceRepeat(const char   s,       Sc10 * c) {return replaceRepeat(&s, 1        ,*c, c->length);}
        Sc10 & replaceRepeat(const char   s,       Sc10 & c) {return replaceRepeat(&s, 1        , c, c. length);}
        Sc10 & replaceRepeat(      char * s, const char   c) {return replaceRepeat( s, strlen(s),&c, 1        );}
        Sc10 & replaceRepeat(      char * s,       char * c) {return replaceRepeat( s, strlen(s), c, strlen(c));}
        Sc10 & replaceRepeat(      char * s,       Sc10 * c) {return replaceRepeat( s, strlen(s),*c, c->length);}
        Sc10 & replaceRepeat(      char * s,       Sc10 & c) {return replaceRepeat( s, strlen(s), c, c. length);}
        Sc10 & replaceRepeat(      Sc10 * s, const char   c) {return replaceRepeat(*s, s->length,&c, 1        );}
        Sc10 & replaceRepeat(      Sc10 * s,       char * c) {return replaceRepeat(*s, s->length, c, strlen(c));}
        Sc10 & replaceRepeat(      Sc10 * s,       Sc10 * c) {return replaceRepeat(*s, s->length,*c, c->length);}
        Sc10 & replaceRepeat(      Sc10 * s,       Sc10 & c) {return replaceRepeat(*s, s->length, c, c. length);}
        Sc10 & replaceRepeat(      Sc10 & s, const char   c) {return replaceRepeat( s, s. length,&c, 1        );}
        Sc10 & replaceRepeat(      Sc10 & s,       char * c) {return replaceRepeat( s, s. length, c, strlen(c));}
        Sc10 & replaceRepeat(      Sc10 & s,       Sc10 * c) {return replaceRepeat( s, s. length,*c, c->length);}
        Sc10 & replaceRepeat(      Sc10 & s,       Sc10 & c) {return replaceRepeat( s, s. length, c, c. length);}

        Sc10 & copyRange(Sc10 & out, size_t from, size_t to) {return out.set(ptr + from, to           );}
        Sc10 & copyRange(Sc10 & out, size_t from           ) {return out.set(ptr + from, length - from);}
        Sc10   copyRange(            size_t from, size_t to) {return Sc10   (ptr + from, to           );}
        Sc10   copyRange(            size_t from           ) {return Sc10   (ptr + from, length - from);}

        Sc10 & removeSymbol(const char symbol) {
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
        Sc10 & removeRange(size_t from, size_t to) {
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
        Sc10 & removeEnd(size_t count) {
            long long len = length - count;
            if (len < 0) length = 0;
            else length = len;
            ptr[length] = 0;
            return *this;
        }

        Sc10 & reset() { return set(nullptr, 0);}

        char * last() {return ptr + length - 1;}

        char & operator [] (size_t argc) {return ptr[argc];}

        Sc10 & operator = (const char   s) { return set(&s, 1        );}
        Sc10 & operator = (const char * s) { return set( s, strlen(s));}
        Sc10 & operator = (      char * s) { return set( s, strlen(s));}
        Sc10 & operator = (      Sc10 & s) { return set( s, s. length);}
        Sc10 & operator = (      Sc10 * s) { return set(*s, s->length);}
        Sc10 & operator = (std::initializer_list<char> list) { return set(list.begin(), list.size());}

        operator       char * () const { return (      char *)ptr;} ;
        operator const char * () const { return (const char *)ptr;} ;

    };
}
#endif
