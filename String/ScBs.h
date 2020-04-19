#ifndef UF_S_ScREPLACE_H
#define UF_S_ScREPLACE_H

#include "BaseLib.h"
#include "../Utils/Bind.h"
#include "../LinuxLike/strnstr.h"
#include "../NewStandart/DynamicArray.h"

//UseFull String module
//Version 1.0 alfa
//Make by Graph Don'te-Crypto

namespace str {

	/*
	Gss - данные строки
	ScBs - обёртка для строки
	Sc - буффер строки вида <value> и обёртка ScBs над ней

	что хочу
	строка от 0 до 500
	можно написать pick(10, 20)
	И вернётся подстрока начиная с 10 элемента и 20 элемент будет считаться последним в строке
	Изменнеие подстроки будет изменять строку

	gss описывает данные о конкретном экзепляре строки,
	то есть, есть один gss и с ним могут работать множество ScBs
	gss создаётся на основе new, "строки" и создания Sc

	Есть gss - описании строки
	Есть ScBs - обёртка над gss, реализующая методы работы с ним. Подключается к существующему gss, не создаёт свой
	Есть Sc<T> - содержит в себе char set[T], наследуется от gss с биндом к set, наследуется от ScBs

	Методы каким-то образом копирующие строку, должны принимать или то, куда копировать, или возвращать Sc<T>

	privet = mir
 
	const char * s1 = "privet"
	const char * s0 = "mir"
	 
	Base privet(s1)
	Base mir(s0)
	 
	Base("Nikolai").pick(2, 4).indexOf(i)
	 
	Base(privet)
	 
	Base l1 = privet.pick(2, 4);

	Base(privet) = mir.pick(2, 4);


	*/

	/*
	StringApapter -> подключается к любым трём наборам
		char * AS начало строки
		интовая переменная AS текущая длинная строки
		интовая переменная AS макисмальный размер строки

	Это может быть как std::string, QString, StringProvider, так и иные

	StringProvider -> существует для хранения литералов, преобразования одного представления строки в другое и так далее
	Если есть std::string, у которого определены уже начало строки, длинна и размер в качестве переменных, то эффективно использовать StringAdapter
	Если есть строка, в которой, допустим, есть только начало строки, то StringProvider нужно использовать в качестве буфера между StringApapter и строкой
	То есть, конечная схема взаимодействия будет выглядеть

	"Литерал" -> StringProvider <-> StringApapter <-> StringWorker

	Если же используется std::string, то

	std::string <-> StringApapter <-> StringWorker

	*/


	
	template <typename demension_type>
	struct StringApapter {
		//use for binding other string to Gss
		char ** ptr   = nullptr; 
		demension_type * length = 0;
		demension_type * size   = 0;

		void bind(char ** xptr, demension_type * xlength, demension_type * xsize) {
			ptr    = xptr;
			length = xlength;
			size   = xsize;
		}
	};

	//GeneralStringState ~ Gss
	struct Gss {
		char c;
		char * ptr    = nullptr;
		size_t length = 0;
		size_t size   = 0;

        Gss *  parent = nullptr;
        nstd::DynamicArray<Gss> slices;

        void bind(char * xptr, size_t xlength, size_t xsize) {ptr = xptr; length = xlength; size = xsize;}


        Gss() {};
        Gss(char * s, size_t length, size_t size, Gss * xparent) {bind(s, length, size); parent = xparent;}
        Gss(char * s, size_t length, size_t size) {bind(s, length, size);}
        Gss(const char * s, size_t length) {bind((char *)s, length, length);}
        Gss(const char * s) {bind((char *) s, strlen(s), strlen(s));} 
        Gss(const char   s) {bind((char *)&c, 1        , 1        ); c = s;};
        Gss(      char * s) {bind( s, strlen(s), strlen(s));}  
        Gss(const Gss  & s) {bind( s.ptr , s.length , s.size );}
        Gss(const Gss  * s) {bind( s->ptr, s->length, s->size);}

        void rightShift(char * from, size_t count) {
        	if (parent) {
        		parent->rightShift(from, count);
        		if (ptr > from) ptr += count;
        		else length += count;
        	}
        	else {
        		Move(from + count, from, length + ptr - from);
        		length += count;
        	}
        }
        void leftShift(char * from, size_t count) {

        }

        Gss delta(size_t dptr, size_t dlen, size_t dsize) const {return Gss(ptr + dptr, length + dlen, size + dsize);}
        size_t relLength(char * xptr) const {if (length - (size_t)xptr + (size_t)ptr > 0) return length - (size_t)xptr + (size_t)ptr; else return 0;}
        size_t relSize  (char * xptr) const {if (size   - (size_t)xptr + (size_t)ptr > 0) return size   - (size_t)xptr + (size_t)ptr; else return 0;}
        Gss pick(size_t from, size_t to) const {return Gss(ptr + from, to - from, to - from, (Gss *)this);}
        operator       char * () const { return (      char *)ptr;}
        operator const char * () const { return (const char *)ptr;}
	};

	struct ScBs {
		Gss   gss_static;
        Gss * gss = &gss_static;

        size_t _calcLen() {
            return gss->length = strlen(gss->ptr);
        }
        size_t _countBytes(bool trust = true) {
            if (trust) return gss->length + 1;
            else return _calcLen() + 1;
        }
        void _printInfo() const {
        	char * buf = new char[gss->length + 1];
            memcpy(buf, gss->ptr, gss->length);
            buf[gss->length] = 0;
            printf("ptr(%lu) -> %s\n    this = %lu\n    parent = %lu\n    size = %lu\n    length = %lu\n", (unsigned long)(size_t)gss->ptr, buf, (long unsigned)(size_t)gss, (long unsigned)(size_t)(gss->parent), (long unsigned)gss->size, (long unsigned)gss->length);
        	delete[] buf;
        }

        ScBs & set(const char * s, size_t len) {
            CopyN(gss->ptr, s, len);
            gss->ptr[len] = 0;
			gss->length = len;
            return *this;
        }
        ScBs & set(const char   s) {return set(&s, 1);}
        ScBs & set(const char * s) {return set( s, strlen(s));}
        ScBs & set(      char * s) {return set( s, strlen(s));}
        ScBs & set(const ScBs & s) {return set( s. gss->ptr, s. gss->length);}
        ScBs & set(const ScBs * s) {return set( s->gss->ptr, s->gss->length);}
        ScBs & set(const Gss  & s) {return set( s .ptr, s. length);}
        ScBs & set(const Gss  * s) {return set( s->ptr, s->length);}

        void bind(const char * s, size_t len, size_t size) {
        	gss->ptr = (char *)s;
        	gss->length = len;
        	gss->size = size;
        }

        ScBs() {}
        ScBs(const char * s, size_t length, size_t size) {bind(s, length, size);}
        ScBs(const char * s, size_t length) {bind(s, length, length);}
        ScBs(const char * s) {bind(s, strlen(s), strlen(s));} 
        ScBs(      char * s) {bind(s, strlen(s), strlen(s));}
        //TODO: need test  
        ScBs(const ScBs & s) {gss_static = *s.gss; gss = &gss_static; gss->parent = s.gss;}
        //TODO: need test
        ScBs(const ScBs * s) {gss = s->gss; gss->parent = s->gss;}
        //TODO: need test
        ScBs(      Gss    s) {gss_static  = s;     gss = &gss_static; gss->parent = s.parent;}
        //TODO: need test
        ScBs(      Gss  * s) {gss = s; gss->parent = s;}

        ~ScBs() {}
        Gss  copy(Gss  & out) const {return ScBs(out).set(this).gss;}
        ScBs copy(ScBs & out) const {return ScBs(out).set(this)    ;}
        //TODO: need test
        ScBs pick(size_t from, size_t to) const { return ScBs(gss->pick(from, to));}

        ScBs & insert(const Gss s, size_t pos) {
            Move(gss->ptr + pos + s.length, gss->ptr + pos, gss->length - pos + 1);
            CopyN(gss->ptr + pos, s.ptr, s.length);
            gss->length += s.length;
            return *this;
        }
        ScBs & append(const Gss s) {
            CopyN(gss->ptr + gss->length, s.ptr, s.length);
            gss->length += s.length;
            gss->ptr[gss->length] = 0;
            return *this;
        }
        ScBs & operator += (const Gss s) {return append(s);}
        ScBs & operator << (const Gss s) {return append(s);}

        ScBs & replace(const Gss what, const Gss with) {
            char * pos = gss->ptr;
            long long shift = with.length - what.length;
            if (shift != 0 ) {
                char * bufptr = nullptr;
                while (( pos = strnstr(pos, what.ptr, gss->relSize(pos)) ) != nullptr) {
                    bufptr = pos + what.length;
                    str::Move(bufptr + shift, bufptr, gss->length + gss->ptr - bufptr + 1); // gss->length + gss->ptr - bufptr == strlen(bufptr)
                    strncpy(pos, with.ptr, with.length);
                    pos += with.length;
                    gss->length += shift;
                }
            }
            else {
                while (( pos = strnstr(pos, what.ptr, gss->relSize(pos)) ) != nullptr) {
                    strncpy(pos, with.ptr, with.length);
                    pos += with.length;
                }
            }
            return *this;
        }
        ScBs & replaceRepeat(const Gss what, const Gss with) {
            char * pos = gss->ptr;
            char * posbuf = nullptr;
            long long shift = with.length - what.length;
            char * bufptr = nullptr;

            if (shift != 0) {
                while (( pos = strnstr(pos, what.ptr, gss->relSize(pos)) ) != nullptr) {
                    posbuf = pos;
                    do {
                        bufptr = posbuf + what.length;
                        memmove(bufptr + shift, bufptr, strlen(bufptr) + 1);
                        strncpy(posbuf, with.ptr, with.length);
                        posbuf += with.length;
                        gss->length += shift;
                    } while (( posbuf = strnstr(posbuf, what.ptr, gss->relSize(posbuf)) ) != nullptr);
                }
            }
            else {
                while (( pos = strnstr(pos, what.ptr, gss->relSize(pos)) ) != nullptr) {
                    posbuf = pos;
                    do {
                        strncpy(posbuf, with.ptr, with.length);
                        posbuf += with.length;
                    }	while (( posbuf = strnstr(posbuf, what.ptr, gss->relSize(posbuf)) ) != nullptr);
                }
            }
            return *this;
        }
        ScBs & removeSymbol(const char symbol) {
            char * buf = gss->ptr;
            char * in = gss->ptr;
            while (true) {
                if (*in != symbol) {
                    *buf = *in;
                    buf++;
                }
                else gss->length--;
                if (*in == 0) break;
                in++;
            }
            return *this;
        }
        ScBs & removeRange(size_t from, size_t to) {
            long long count = gss->length - to;
            if (count <= 0) {
                gss->ptr[from] = 0;
                gss->length = from;
            }
            else {
                Move(gss->ptr + from, gss->ptr + to + 1, (size_t)count);
                gss->length -= to + 1 - from;
            }
            return *this;
        }
        ScBs & removeEnd(size_t count) {
            long long len = gss->length - count;
            if (len < 0) gss->length = 0;
            else gss->length = len;
            return *this;
        }

        ScBs & rightShift(size_t index, size_t count) {
        	gss->rightShift(gss->ptr + index, count);
        	return *this;
        }

        ScBs & reset() { return set("", 0);}

        char * last() {return gss->ptr + gss->length - 1;}

        char & operator [] (size_t argc) const {return gss->ptr[argc];}
        ScBs & operator = (ScBs   sc) {return this->set( sc);}
        ScBs & operator = (ScBs * sc) {return this->set(*sc);}

        //For std::map 
        bool operator < (const ScBs & sc) const {
        	if (strncmp(sc.gss->ptr, gss->ptr, gss->length) < 0) return true;
            else return false;
        }

        operator       char * () const { return (      char *)gss->ptr;}
        operator const char * () const { return (const char *)gss->ptr;}
        operator       Gss  & () const { return *gss;}
        operator const Gss  & () const { return *gss;}

        bool equals(const Gss * s) const {
        	if (strncmp(gss->ptr, s->ptr, s->length) == 0) return true;
            else return false;
        }
        bool equals      (const Gss s) const {return equals(&s);} 

        bool operator == (const Gss s) const {return equals(&s);}
    };

	template <const size_t T>
    struct Sc : public ScBs {
        char ptr[T];
    };
}

#endif