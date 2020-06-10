#ifndef UF_U_Context_H
#define UF_U_Context_H

#include "Bytes.hpp"

//UseFull Utils Context stack module 
//Version 1.0 alfa
//Make by Graph Don'te-Crypto

namespace utils {
	class Context {
	public:
	    size_t context_adr[16];
	    size_t context_argc;
	    char context[256];
	    Context() {
	        context_adr[0] = 0;
	        context_argc = 0;
	    }
	    ~Context() {
	    }
	    size_t size(size_t argc) {
	        return context_adr[argc+1] - context_adr[argc];
	    }
	    char * adr(size_t argc) {
	        return context + context_adr[argc];
	    }
	    template <typename B>
		bool addRef(B & some, size_t size = sizeof(B)) {
	        if (context_argc < 16 && context_adr[context_argc] + size < 256) {
	            memcpy(adr(context_argc), &some, size);
	            context_adr[context_argc + 1] = context_adr[context_argc] + size;
	            context_argc++;
	            return true;
	        }
	        else return false;
	    }
	    template <typename B>
	    bool addValue(B some, size_t size = sizeof(B)) {return addRef(some,  size        );}
	    bool addString (const char * some)             {return addRef(*some, strlen(some));}
	    bool addString (      char * some)             {return addRef(*some, strlen(some));}
	    void * get(void * ptr, size_t argc) {fromBytes(ptr, adr(argc), size(argc));return ptr;}
	    void * get(char * ptr, size_t argc) {return get((void *)ptr, argc);}
	    char * operator[](size_t argc) {return adr(argc);}
	    template <typename B>
	    B getValue(size_t argc) {
	        B buf;
	        get(&buf, argc);
	        return buf;
	    }
	    template <typename B>
	    B & getRef(size_t argc) {
	        return *((B *)(adr(argc)));
	    }
	};
	//if function fun1() return ContextCopy<SomeClass>
	//and we
	//SomeClass someClass = fun1();
	//someClass was copy of SomeClass ib fun1();

	template <typename T>
	class ContextCopy : public Context {
	public:
	    T record;
	    ContextCopy(T   t) : Context() {record = t;}
	    ContextCopy(     ) : Context() {}
	    ~ContextCopy() {}
	    operator T () {return record;}
	};

	template <typename T>
	class ContextPtr : public Context {
	public:
	    T * record = nullptr;
	    ContextPtr(T & t) : Context() {record = &t;}
	    ContextPtr(T * t) : Context() {record =  t;}
	    ~ContextPtr() {}
	    operator T   () {return *record;}
	    operator T * () {return  record;}
	    operator ContextCopy<T> () {
	        return ContextCopy<T>(*record);
	    }
	};
}

#endif