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
		bool source = false;
		
		void set(const Type * ptr, size_t length, bool source = false) {
			this->ptr = ptr;
			this->length = length;
			this->source = source;
		}
		
		Span(const Span & span) {
			//Если в конструктор был передан оригинальный Span - новый Span оригинальный, старый - нет
			//Иначе оба не оригинальные
			set(span.ptr, span.length, span.source);
			if (span.source) span.source = false;
		}
		
		Span(const Type * ptr, size_t length, bool source = false) {
			set(ptr, length, source);
		}
				
		Span() {
			//По умолчанию все Span - не оригинальные
			set(nullptr, 0, false);
		}
		~Span() {
			//Если Span - оригинальный, то он освобождает память при удалении
			if (source) delete[] ptr;
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