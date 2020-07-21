#ifndef UF_T_Span_H
#define UF_T_Span_H

//UseFull Templates Span module
//Version 1.0 alfa
//Make by Graph Don'te-Crypto

#include "../Utils/Concepts.hpp"

namespace uft {
	//Readonly array
	template <typename Type>
	struct Span {
		const Type * ptr = nullptr;
		size_t length = 0;
		
		void set(const Type * ptr, size_t length) {
			this->ptr = ptr;
			this->length = length;
		}
		
		Span(const Span & span) {
			set(span.ptr, span.length);
		}
		
		Span(const Type * ptr, size_t length) {
			set(ptr, length);
		}
				
		Span() {
			set(nullptr, 0);
		}
		
		const Type & operator [] (size_t index) const {return ptr[index];}
		
		void foreach(CoLambda<void, const Type &> auto lambda) {
			for (size_t i = 0; i < length; i++)
				lambda(ptr[i]);
		}
		
		Type & getUnsafe(size_t index) {return ((Type *)ptr)[index];}
	};
}

#endif