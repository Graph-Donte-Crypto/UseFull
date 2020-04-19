#ifndef UF_S_Sc_H
#define UF_S_Sc_H

#include <initializer_list>

#include "BaseLib.h"
#include "Compare.h"

//UseFull String Constant length(T bytes) module
//Version 1.2 alfa
//Make by Graph Don'te-Crypto

namespace str {

    struct ScBs {
        size_t * size = nullptr;
        char * ptr = nullptr;
        size_t * length = nullptr;
        operator       char * () const { return (      char *)ptr;} ;
        operator const char * () const { return (const char *)ptr;} ;
    };

    template <const size_t T>
    struct Sc : public ScBs {
        size_t size = T;
        size_t length = 0;
        char ptr[T];

        template <const size_t R>
        Sc<R>   to() const {return  Sc<R>   (this);}
        template <const size_t R>
        Sc<R> * as() {return (Sc<R> *)(this);}

        void _bindScBs() {
            ScBs::size = &size;
            ScBs::ptr = ptr;
            ScBs::length = &length;
        }
        size_t _calcLen() {
            return length = strlen(ptr);
        }
        size_t _countBytes(bool trust = true) {
            if (trust) return length + 1;
            else return _calcLen() + 1;
        }
        void _printInfo() const {
            printf("ptr(%lu) -> %s\n    size = %lu\n    length = %lu\n", (long unsigned)((size_t)ptr), ptr, (long unsigned)size, (long unsigned)length);
        }

        Sc & set(const char * s, size_t len) {
            CopyN(ptr, s, len);
            ptr[len] = 0;
			length = len;
            return *this;
        }
        Sc & set(const char   s) {return set(&s, 1);}
        Sc & set(const char * s) {return set( s, strlen(s));}
        Sc & set(      char * s) {return set( s, strlen(s));}
        Sc & set(      ScBs * s) {return set(*s,*s->length);}
        Sc & set(      ScBs & s) {return set( s,*s. length);}

        template <size_t Q>
        Sc<T> & set(Sc<Q> & sc) {return this->set(sc.ptr, sc.length);}

        Sc() { _bindScBs(); ptr[0] = 0;}
        Sc(const char * s, size_t s_length) { _bindScBs(); set( s, s_length );}
        Sc(const char * s) { _bindScBs(); set( s, strlen(s));}
        Sc(      char * s) { _bindScBs(); set( s, strlen(s));}
        Sc(const ScBs & s) { _bindScBs(); set( s,*s. length);}
        Sc(const Sc   & s) { _bindScBs(); set( s, s. length);}
        Sc(const ScBs * s) { _bindScBs(); set(*s,*s->length);}
        Sc(const Sc   * s) { _bindScBs(); set(*s, s->length);}
        Sc(std::initializer_list<char> list) {
             _bindScBs();
            length = list.size();
            CopyN(ptr, list.begin(), length);
        }

        ~Sc() {}
        Sc copy() const {
            return Sc(this);
        }

        Sc & insert(const char * s, size_t len, size_t pos) {
            Move(ptr + pos + len, ptr + pos, length - pos + 1);
            CopyN(ptr + pos, s, len);
            length += len;
            return *this;
        }
        Sc & insert(const char   s, size_t pos) {return insert(&s, 1        , pos);}
        Sc & insert(      char * s, size_t pos) {return insert( s, strlen(s), pos);}
        Sc & insert(      ScBs * s, size_t pos) {return insert(*s,*s->length, pos);}
        Sc & insert(      ScBs & s, size_t pos) {return insert( s,*s. length, pos);}

        Sc & append(const char * s, size_t len) {
            CopyN(ptr + length, s, len);
            length += len;
            ptr[length] = 0;
            return *this;
        }
        Sc & append(const char   s) {return append(&s, 1);}
        Sc & append(const char * s) {return append( s, strlen(s));}
        Sc & append(      char * s) {return append( s, strlen(s));}
        Sc & append(      ScBs * s) {return append(*s,*s->length);}
        Sc & append(      ScBs & s) {return append( s,*s. length);}

        Sc & operator += (      char * s) {return append(s);}
        Sc & operator += (const char * s) {return append(s);}
        Sc & operator += (      char   s) {return append(s);}
        Sc & operator += (      ScBs & s) {return append(s);}
        Sc & operator += (      ScBs * s) {return append(s);}

        Sc & operator << (      char * s) {return append(s);}
        Sc & operator << (const char * s) {return append(s);}
        Sc & operator << (      char   s) {return append(s);}
        Sc & operator << (      ScBs & s) {return append(s);}
        Sc & operator << (      ScBs * s) {return append(s);}

        Sc & replace(const char * what, size_t what_l, const char * with, size_t with_l) {
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

        Sc & replace(const char   s, const char   c) {return replace(&s, 1        ,&c, 1        );}
        Sc & replace(const char   s,       char * c) {return replace(&s, 1        , c, strlen(c));}
        Sc & replace(const char   s,       ScBs * c) {return replace(&s, 1        ,*c,*c->length);}
        Sc & replace(const char   s,       ScBs & c) {return replace(&s, 1        , c,*c. length);}
        Sc & replace(      char * s, const char   c) {return replace( s, strlen(s),&c, 1        );}
        Sc & replace(      char * s,       char * c) {return replace( s, strlen(s), c, strlen(c));}
        Sc & replace(      char * s,       ScBs * c) {return replace( s, strlen(s),*c,*c->length);}
        Sc & replace(      char * s,       ScBs & c) {return replace( s, strlen(s), c,*c. length);}
        Sc & replace(      ScBs * s, const char   c) {return replace(*s,*s->length,&c, 1        );}
        Sc & replace(      ScBs * s,       char * c) {return replace(*s,*s->length, c, strlen(c));}
        Sc & replace(      ScBs * s,       ScBs * c) {return replace(*s,*s->length,*c,*c->length);}
        Sc & replace(      ScBs * s,       ScBs & c) {return replace(*s,*s->length, c,*c. length);}
        Sc & replace(      ScBs & s, const char   c) {return replace( s,*s. length,&c, 1        );}
        Sc & replace(      ScBs & s,       char * c) {return replace( s,*s. length, c, strlen(c));}
        Sc & replace(      ScBs & s,       ScBs * c) {return replace( s,*s. length,*c,*c->length);}
        Sc & replace(      ScBs & s,       ScBs & c) {return replace( s,*s. length, c,*c. length);}

        Sc & replaceRepeat(const char * what, size_t what_l, const char * with, size_t with_l) {
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

        Sc & replaceRepeat(const char   s, const char   c) {return replaceRepeat(&s, 1        ,&c, 1        );}
        Sc & replaceRepeat(const char   s,       char * c) {return replaceRepeat(&s, 1        , c, strlen(c));}
        Sc & replaceRepeat(const char   s,       ScBs * c) {return replaceRepeat(&s, 1        ,*c,*c->length);}
        Sc & replaceRepeat(const char   s,       ScBs & c) {return replaceRepeat(&s, 1        , c,*c. length);}
        Sc & replaceRepeat(      char * s, const char   c) {return replaceRepeat( s, strlen(s),&c, 1        );}
        Sc & replaceRepeat(      char * s,       char * c) {return replaceRepeat( s, strlen(s), c, strlen(c));}
        Sc & replaceRepeat(      char * s,       ScBs * c) {return replaceRepeat( s, strlen(s),*c,*c->length);}
        Sc & replaceRepeat(      char * s,       ScBs & c) {return replaceRepeat( s, strlen(s), c,*c. length);}
        Sc & replaceRepeat(      ScBs * s, const char   c) {return replaceRepeat(*s,*s->length,&c, 1        );}
        Sc & replaceRepeat(      ScBs * s,       char * c) {return replaceRepeat(*s,*s->length, c, strlen(c));}
        Sc & replaceRepeat(      ScBs * s,       ScBs * c) {return replaceRepeat(*s,*s->length,*c,*c->length);}
        Sc & replaceRepeat(      ScBs * s,       ScBs & c) {return replaceRepeat(*s,*s->length, c,*c. length);}
        Sc & replaceRepeat(      ScBs & s, const char   c) {return replaceRepeat( s,*s. length,&c, 1        );}
        Sc & replaceRepeat(      ScBs & s,       char * c) {return replaceRepeat( s,*s. length, c, strlen(c));}
        Sc & replaceRepeat(      ScBs & s,       ScBs * c) {return replaceRepeat( s,*s. length,*c,*c->length);}
        Sc & replaceRepeat(      ScBs & s,       ScBs & c) {return replaceRepeat( s,*s. length, c,*c. length);}

        template <size_t R>
        Sc<R> & copyRange(Sc<R> & out, size_t from, size_t to) const {return out.set(ptr + from, to           );}
        template <size_t R>
        Sc<R> & copyRange(Sc<R> & out, size_t from           ) const {return out.set(ptr + from, length - from);}

        Sc<T> & copyRange(Sc<T> & out, size_t from, size_t to) const {return out.set(ptr + from, to           );}
        Sc<T> & copyRange(Sc<T> & out, size_t from           ) const {return out.set(ptr + from, length - from);}

        Sc<T>   copyRange(             size_t from, size_t to) const {return Sc<T>  (ptr + from, to           );}
        Sc<T>   copyRange(             size_t from           ) const {return Sc<T>  (ptr + from, length - from);}

        Sc<T> & removeSymbol(const char symbol) {
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
        Sc<T> & removeRange(size_t from, size_t to) {
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
        Sc<T> & removeEnd(size_t count) {
            long long len = length - count;
            if (len < 0) length = 0;
            else length = len;
            ptr[length] = 0;
            return *this;
        }

        Sc<T> & reset() { return set("", 0);}

        char * last() {return ptr + length - 1;}

        char & operator [] (size_t argc) const {return ptr[argc];}

        //Sc & operator = (const char   s) { return set(&s, 1        );}
        //Sc & operator = (      char * s) { return set( s, strlen(s));}
        //Sc & operator = (      ScBs & s) { return set( s,*s. length);}
        //Sc & operator = (      ScBs * s) { return set(*s,*s->length);}
        //Sc & operator = (std::initializer_list<char> list) { return set(list.begin(), list.size());}

        Sc   & operator = (    ScBs sc) {return this->set( sc);}

        template <size_t Q>
        Sc<T> & operator = (Sc<Q>   sc) {return this->set( sc);}
        template <size_t Q>
        Sc<T> & operator = (Sc<Q> * sc) {return this->set(*sc);}

        //For std::map
        bool operator < (const Sc & sc) const {
            if (str::CompareStrong(ptr, sc.ptr) == -1) return true;
            else return false;
        }

        operator       char * () const { return (      char *)ptr;} ;
        operator const char * () const { return (const char *)ptr;} ;

        bool equals(const char * s, size_t len) const {
            if (strncmp(ptr, s, len) == 0) return true;
            else return false;
        }

        bool equals (      char * s) const {return equals( s, strlen(s));}
        bool equals (const char * s) const {return equals( s, strlen(s));}
        bool equals (      char   s) const {return equals(&s, 1        );}
        bool equals (      ScBs & s) const {return equals( s,*s.length );}
        bool equals (      ScBs * s) const {return equals(*s,*s->length);}

        bool operator == (      char * s) const {return equals( s, strlen(s));}
        bool operator == (const char * s) const {return equals( s, strlen(s));}
        bool operator == (      char   s) const {return equals(&s, 1        );}
        bool operator == (      ScBs & s) const {return equals( s,*s.length );}
        bool operator == (      ScBs * s) const {return equals(*s,*s->length);}
		
		bool contains(char s) const {return (bool)strchr(ptr, s);}

    };


}
#endif
