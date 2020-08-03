#ifndef UF_T_Span_H
#define UF_T_Span_H

//UseFull Templates Span module
//Version 1.0 alfa
//Make by Graph Don'te-Crypto

#include "../Utils/Concepts.hpp"
#include "../Utils/Macro.hpp"
#include <cstddef>
#include <functional>
#include "stdio.h"

namespace uft {
	
	using namespace utils;
	
	//Readonly array
	template <typename Type>
	struct Span {
		const Type * ptr = nullptr;
		size_t length = 0;
		bool owner = false;
		
		std::function<void(Span<Type> *)> dispose;
		bool inherited_dispose = false;
		bool is_dispose = false;
		
		void setDisposeFunction(CoLambda<void, Span<Type> *> auto lambda) {
			dispose = lambda;
			is_dispose = true;
		}
		
		void set(const Type * ptr, size_t length, bool owner = false) {
			this->ptr = ptr;
			this->length = length;
			this->owner = owner;
		}
			
		void constructor(Span<Type> & span) {
			
			if (span.inherited_dispose) {
				//Если объект должен был сам вызывать dispose - это будем делать мы
				//Иначе оба объекта не сами вызывать будут
				is_dispose = span.is_dispose;
				span.is_dispose = false;
				
				dispose = std::move(span.dispose);
				inherited_dispose = true;
			}
			else {
				if (span.is_dispose && span.owner) span.dispose(&span);
			}
			
			set(span.ptr, span.length, span.owner);
			if (span.owner) span.owner = false;
			//Если dispose наследуется
			
		}
		
		void destructor() {
			//Если Span - оригинальный, то он освобождает память при удалении
			if (owner) {
				if (is_dispose) dispose(this);
				else deleteData();
			}
		}
		
		/*
		
		ptr_cast - это очень плохо
		Особенно для действия вида const Type -> Type
		Но я не помню, как сделать функцию, которая принимает rvalue
		Поэтому пока тут ptr_cast
		
		*/	
		
		//TODO: REMOVE ptr_cast && add rvalue variants
		Span(const Span<Type> & span) {
			Span<Type> & no_const_span = ptr_cast(Span<Type>, span);
			constructor(no_const_span);
		}
		
		Span<Type> & operator = (const Span<Type> & span) {
			destructor();
			Span<Type> & no_const_span = ptr_cast(Span<Type>, span);
			constructor(no_const_span);
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
		}
		
		~Span() {
			destructor();
		}
		
		const Type & operator [] (size_t index) const {return ptr[index];}
		
		void foreach(CoLambda<void, const Type &> auto lambda) {
			for (size_t i = 0; i < length; i++)
				lambda(ptr[i]);
		}
		
		void foreach(CoLambda2<void, const Type &, size_t> auto lambda) {
			for (size_t i = 0; i < length; i++)
				lambda(ptr[i], i);
		}
		
		//Unsafe на то и Unsafe, чтобы была дыра в безопасности и возможность изменять данные в Span
		
		Type & getUnsafe(size_t index) {return ((Type *)ptr)[index];}
	};
}

#endif