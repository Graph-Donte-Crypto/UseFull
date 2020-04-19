#ifndef UF_S_Parser_H
#define UF_S_Parser_H

#include <stdio.h>
#include <stdlib.h>

//UseFull Utils Parser module
//Version 1.0 alfa
//Make by Graph Don'te-Crypto

namespace utils {
	template <typename T>
	struct Parser {
		T * base = nullptr;
	    size_t index = -1;
	    size_t length = 0;
	    Parser(T * base, size_t length) {
	    	this->base   = base;
	    	this->length =length;
	    }

	    T * getNextPtr() {
	        index++;
	        if (index     == length) return nullptr;
	        else                     return  base + index;
	    }
	    T & getNextRef() {
	    	index++;
	        if (index     == length) {printf("Parser::getNextRef::Error\n"); exit(1);}
	        else 	                 return *(base + index);
	    }
	    void skip() {
	    	index++;
	        if (index     == length) {printf("Parser::skip::Error\n"); exit(1);}
	    }
	    T * nextPtr() {
	        if (index + 1 == length) return nullptr;
	        else                     return  base + index + 1;
	    }
	    T & nextRef() {
	    	if (index + 1 == length) {printf("Parser::nextRef::Error\n"); exit(1);}
	    	else                     return *(base + index + 1);
	    }
	    bool isNext() {
	        if (index + 1 == length) return false;
	        else                     return true;
	    }
	    

	    T * operator [] (size_t index) {
	    	return base + index;
	    }
	};
}



#endif
