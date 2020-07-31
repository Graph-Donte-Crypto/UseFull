#ifndef UF_T_Span_H
#define UF_T_Span_H

//UseFull Templates Span module
//Version 1.0 alfa
//Make by Graph Don'te-Crypto

#include "../Utils/Concepts.hpp"
#include "../Utils/Macro.hpp"
#include <cstddef>
#include <stdio.h>

namespace uft {
	
	using namespace utils;
	
	//Readonly array
	template <typename Type>
	struct Span {
		const Type * ptr = nullptr;
		size_t length = 0;
		bool owner = false;
		
		void set(const Type * ptr, size_t length, bool owner = false) {
			this->ptr = ptr;
			this->length = length;
			this->owner = owner;
		}
		
		/*
		
		ptr_cast - это очень плохо
		Особенно для действия вида const Type -> Type
		Но я не помню, как сделать функцию, которая принимает rvalue
		Поэтому пока тут ptr_cast
		
		*/
		
		/*
		//Если был передан оригинальный Span - новый Span оригинальный, старый - нет
		//Иначе оба не оригинальные
		*/
		
		//TODO: REMOVE ptr_cast && add rvalue variants
		Span(const Span<Type> & span) {
			
			set(span.ptr, span.length, span.owner);
			if (span.owner) ptr_cast(Span<Type>, span).owner = false;
		}
		
		Span<Type> & operator = (const Span<Type> & span) {
			if (owner) deleteData();
			set(span.ptr, span.length, span.owner);
			if (span.owner) ptr_cast(Span<Type>, span).owner = false;
			return *this;
		}
		//
		
		Span(const Type * ptr, size_t length, bool owner = false) {
			set(ptr, length, owner);
		}
				
		Span() {
			//По умолчанию все Span - не оригинальные
			set(nullptr, 0, false);
		}
		
		void deleteData() {
			delete[] ptr;
			ptr = nullptr;
			length = 0;
			owner = false;
			printf("Deleted\n");
		}
		
		~Span() {
			//Если Span - оригинальный, то он освобождает память при удалении
			if (owner) deleteData();
		}
		
		const Type & operator [] (size_t index) const {return ptr[index];}
		
		void foreach(CoLambda<void, const Type &> auto lambda) {
			for (size_t i = 0; i < length; i++)
				lambda(ptr[i]);
		}
		
		//Unsafe на то и Unsafe, чтобы была дыра в безопасности и возможность изменять данные в Span
		
		Type & getUnsafe(size_t index) {return ((Type *)ptr)[index];}
	};
}

#endif