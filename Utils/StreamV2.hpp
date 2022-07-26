#ifndef UF_U_Stream_H
#define UF_U_Stream_H

#include "StdDiagnosticIgnore.hpp"

#include <cstddef>
#include <cstdio>
#include <utility>
#include <stdio.h>

#include "StdDiagnosticIgnoreEnd.hpp"

//UseFull Utils Stream api module
//Version 1.0 alfa
//Make by Graph Don'te-Crypto

//#define STREAM_DEBUG

#define RUNTIME_DEBUG 1

#include "RuntimeDebug.hpp"
#include "Concepts.hpp"
#include "Ok.hpp"

#ifdef RUNTIME_DEBUG
#include "StdDiagnosticIgnore.hpp"
#include <cxxabi.h>
#include "StdDiagnosticIgnoreEnd.hpp"
#endif // RUNTIME_DEBUG

namespace utils {

	template <typename PrintTypeType>
	constexpr void PrintType() {
		typedef typename PrintTypeType::PrintType X;
		//struct Anon {} a = PrintTypeType();
	}

	DEBUG(
	template <typename Type>
	constexpr const char * GetType(Type * pointer = nullptr) {
		int status;
		return abi::__cxa_demangle(typeid(Type).name(), 0, 0, &status);
	}
	)

	template <typename Type>
	struct StreamIndexElement {
		Type * element = nullptr;
		size_t index = 0;
		Type & Element() {
			DEBUG(
				if (element == nullptr)
					printf("StreamIndexElement::Element::Error: element == nullptr on getting Element\n");
			)
			return *element;
		}
		StreamIndexElement() : element(nullptr), index(0) {}
		StreamIndexElement(Type & element, size_t index) : element(&element), index(index) {}
		StreamIndexElement(const StreamIndexElement & sie) : element(sie.element), index(sie.index) {}
		StreamIndexElement & operator = ( StreamIndexElement & a) {
			index = a.index;
			element = a.element;
			return *this;
		}
		StreamIndexElement & operator = ( StreamIndexElement && a) {
			index = a.index;
			element = a.element;
			return *this;
		};
	};
	

	template <typename RealType, typename SourceType, typename ItemType, typename InheritItemType = ItemType>
		//requires CoIterable<SourceType, ItemType>
	struct Stream;

	template <typename SourceType, typename ItemType, typename Lambda, typename OldItemType>
		requires CoIterable<SourceType, OldItemType>
	struct StreamSelect;

	template <typename SourceType, typename ItemType, typename Lambda, typename OldItemType>
		requires CoIterable<SourceType, OldItemType>
	struct StreamSelectWithIndex;

	template <typename SourceType, typename ItemType, typename Lambda>
		requires CoIterable<SourceType, ItemType>
	struct StreamFilter;

	template <typename SourceType, typename ItemType>
		requires CoIterable<SourceType, ItemType>
	struct StreamIgnore;

	template <typename SourceType, typename ItemType>
		requires CoIterable<SourceType, ItemType>
	struct StreamAccept;

	template <typename SourceType, typename ItemType, typename Lambda>
		requires CoIterable<SourceType, ItemType>
	struct StreamWhile;

	template <typename SourceTypeA, typename SourceTypeB, typename ItemType>
		//requires (CoIterable<SourceTypeA, ItemType> && CoIterable<SourceTypeB, ItemType>)
	struct StreamConcat;

	template <typename TypeFrom>
	constexpr auto GetStreamFromIterator(CoIterable<TypeFrom> auto & source);

	//Базовый Fluent объект.
	//Не должен создаваться обычными способами
	//SelfType - настоящий тип себя, который получается из наследника. Нужен для того, чтобы можно было вызывать методы базового класса из наследника
	//SourceType - тип сурса, из которого мы будем брать элементы
	//ItemType - тип заглушка. Влияние описать сложно, но ItemType - очень важен.
	//InheritItemType - настоящий тип, почти всегда равен ItemType
	//Разница между ними примерно такая
	//если мы берём this, то настоящий тип - ItemType
	//если мы берём self, то настоящий тип - InheritItemType
	template <typename SelfType, typename SourceType, typename ItemType, typename InheritItemType>
		//requires CoIterable<SourceType, ItemType>
	struct Stream {
		#define self (*((SelfType *)this))

		DEBUG(
			const char * self_type = "NULL";
			const char * item_type = "NULL";
			const char * source_type = "NULL";
			const char * this_type = "NULL";
			const char * inherit_item_type = "NULL";
		)
		SourceType source;

		constexpr bool hasNext() {return source.hasNext();}
		constexpr ItemType & getNext() {return source.getNext();}
		constexpr ItemType & getCurr()  {return source.getCurr();}
		constexpr void restore() { source.restore(); }
		constexpr void prepare() { source.prepare(); }

		constexpr void DebugInfo() {
			DEBUG(
				source_type = GetType<SourceType>();
				this_type = GetType<decltype(this)>();
				self_type = GetType<SelfType>();
				item_type = GetType<ItemType>();
				inherit_item_type = GetType<InheritItemType>();
			)
		}

		constexpr Stream(const SourceType &  _source, ItemType *) : source(_source) {DebugInfo();}
		constexpr Stream(const SourceType && _source, ItemType *) : source(_source) {DebugInfo();}

		constexpr void ForEach(CoLambda<void, InheritItemType> auto lambda) {
			self.prepare();
			while (self.hasNext())
				lambda(self.getNext());
			self.restore();
		}
		constexpr Ok<InheritItemType> First() {
			self.prepare();
			Ok<InheritItemType> result = self.hasNext() 
				? Ok<InheritItemType>(self.getNext()) 
				: Ok<InheritItemType>();
			self.restore();
			return result;;
		}
		constexpr InheritItemType Reduce(const CoLambda<InheritItemType, InheritItemType, InheritItemType> auto lambda, InheritItemType && base = InheritItemType()) {
			self.prepare();
			while (self.hasNext())
				base = lambda(base, self.getNext());
			self.restore();
			return base;
		}
		constexpr InheritItemType Reduce(const CoLambda<InheritItemType, InheritItemType, InheritItemType> auto lambda, InheritItemType & base) {
			self.prepare();
			while (self.hasNext())
				base = lambda(base, self.getNext());
			self.restore();
			return base;
		}
		template <typename NewItemType>
		constexpr NewItemType Reduce(const CoLambda<NewItemType, NewItemType, InheritItemType> auto lambda, NewItemType && base = NewItemType()) {	
			self.prepare();
			while (self.hasNext())
				base = lambda(base, self.getNext());
			self.restore();
			return base;
		}
		template <typename NewItemType>
		constexpr NewItemType Reduce(const CoLambda<NewItemType, NewItemType, InheritItemType> auto lambda, NewItemType & base) {	
			self.prepare();
			while (self.hasNext())
				base = lambda(base, self.getNext());
			self.restore();
			return base;
		}

		constexpr auto SelectPointer() {
			return StreamSelect(self, (InheritItemType **)0, [](auto & x) {return &x;}, (InheritItemType *)0);
		}
		constexpr auto Select(const CoLambdaSingle<InheritItemType, InheritItemType> auto lambda) {
			return StreamSelect(self, (InheritItemType *)0, lambda, (InheritItemType *)0);
		}
		template <typename NewItemType>
		constexpr auto Select(const CoLambdaSingle<NewItemType, InheritItemType> auto lambda) {
			return StreamSelect(self, (NewItemType *)0, lambda, (InheritItemType *)0);
		}
		constexpr auto SelectWithIndex() {
			return StreamSelectWithIndex(self, (StreamIndexElement<InheritItemType> *)0, [](auto & x){return x;}, (InheritItemType *)0);
		}
		constexpr auto SelectWithIndex(const CoLambdaSingle<StreamIndexElement<InheritItemType>, StreamIndexElement<InheritItemType>> auto lambda) {
			return StreamSelectWithIndex(self, (StreamIndexElement<InheritItemType> *)0, lambda, (InheritItemType *)0);
		}
		template <typename NewItemType>
		constexpr auto SelectWithIndex(const CoLambdaSingle<NewItemType, StreamIndexElement<InheritItemType>> auto lambda) {
			return StreamSelectWithIndex(self, (NewItemType *)0, lambda, (InheritItemType *)0);
		}
		constexpr auto Filter(const CoLambda<bool, InheritItemType> auto lambda) {
			return StreamFilter(self, (InheritItemType *)0, lambda);
		}
		//Работает через prepare
		constexpr auto Ignore(size_t count) {
			return StreamIgnore(self, (InheritItemType *)0, count);
		}
		constexpr auto Accept(size_t count) {
			return StreamAccept(self, (InheritItemType *)0, count);
		}
		//При вызове сработает prepare для двух итераторов
		constexpr auto Concat(CoIterable<InheritItemType> auto iterator) {
			return StreamConcat(self, iterator, (InheritItemType *)0);
		}
		

		constexpr void Execute() {
			while (self.hasNext())
				self.getNext();
			self.restore();
		}

		~Stream() {}
	};

	#define parent Stream<StreamSelect<SourceType, ItemType, Lambda, OldItemType>, SourceType, OldItemType, ItemType>
	template <typename SourceType, typename ItemType, typename Lambda, typename OldItemType>
		requires CoIterable<SourceType, OldItemType>
	struct StreamSelect : public parent {
		ItemType placeholder;
		const Lambda lambda;
		typedef ItemType SelectItemType;

		StreamSelect(SourceType &  coll, ItemType *, const Lambda lambda, OldItemType *)
			: parent(coll, 0)
			, lambda(lambda)
			{}
		StreamSelect(SourceType && coll, ItemType *, const Lambda lambda, OldItemType *)
			: parent(coll, 0)
			, lambda(lambda)
			{}

		ItemType & getNext() {
			placeholder = lambda(parent::getNext());
			return placeholder;
		}
		ItemType & getCurr() {return placeholder;}
	};
	#undef parent

	#define parent Stream<StreamSelectWithIndex<SourceType, ItemType, Lambda, OldItemType>, SourceType, OldItemType, ItemType>
	template <typename SourceType, typename ItemType, typename Lambda, typename OldItemType>
		requires CoIterable<SourceType, OldItemType>
	struct StreamSelectWithIndex : public parent {
		ItemType placeholder;
		size_t num = 0;
		const Lambda lambda;
		typedef ItemType SelectItemType;

		StreamSelectWithIndex(SourceType &  coll, ItemType *, const Lambda lambda, OldItemType *)
			: parent(coll, 0)
			, lambda(lambda)
			{}
		StreamSelectWithIndex(SourceType && coll, ItemType *, const Lambda lambda, OldItemType *)
			: parent(coll, 0)
			, lambda(lambda)
			{}

		ItemType & getNext() {
			auto sie = StreamIndexElement(parent::getNext(), num++);
			placeholder = lambda(sie);
			return placeholder;
		}
		ItemType & getCurr() {return placeholder;}
	};
	#undef parent

	#define parent Stream<StreamFilter<SourceType, ItemType, Lambda>, SourceType, ItemType>
	template <typename SourceType, typename ItemType, typename Lambda>
		requires CoIterable<SourceType, ItemType>
	struct StreamFilter : public parent {
		const Lambda lambda;

		bool nextGetted = true;
		ItemType placeholder;

		constexpr StreamFilter(SourceType &  coll, ItemType *, const Lambda lambda)
			: parent(coll, (ItemType *)0)
			, lambda(lambda)
			{}
		constexpr StreamFilter(SourceType && coll, ItemType *, const Lambda lambda)
			: parent(coll, (ItemType *)0)
			, lambda(lambda)
			{}

		constexpr bool hasNext() {
			//Идём по циклу пока: нужен новый элемент и новые элементы есть в принципе
			while (nextGetted && parent::hasNext()) {
				placeholder = parent::getNext();
				//Если lambda(placeholder) == true, то элемент нам подходит, и новый элемент брать не нужно
				nextGetted = !lambda(placeholder);
			}
			return !nextGetted;
		}
		constexpr ItemType & getNext() {
			nextGetted = true;
			return placeholder;
		}

		constexpr ItemType & getCurr() {
			return placeholder;
		}

	};
	#undef parent

	#define parent Stream<StreamIgnore<SourceType, ItemType>, SourceType, ItemType>
	template <typename SourceType, typename ItemType>
		requires CoIterable<SourceType, ItemType>
	struct StreamIgnore : public parent {

		size_t ignore;

		constexpr void prepare() {
			while (ignore != 0 && parent::hasNext()) {
				ignore -= 1;
				parent::getNext();
			}
		}

		constexpr StreamIgnore(SourceType & _source, ItemType *, size_t num)
			: parent(_source, 0),
			  ignore(num)
		{}

		constexpr StreamIgnore(SourceType && _source, ItemType *, size_t num)
			: parent(_source, 0),
			  ignore(num)
		{}
	};
	#undef parent

	#define parent Stream<StreamAccept<SourceType, ItemType>, SourceType, ItemType>
	template <typename SourceType, typename ItemType>
		requires CoIterable<SourceType, ItemType>
	struct StreamAccept : public parent {

		size_t accept;

		constexpr StreamAccept(SourceType & _source, ItemType *, size_t num)
			: parent(_source, 0),
			  accept(num)
		{}

		constexpr StreamAccept(SourceType && _source, ItemType *, size_t num)
			: parent(_source, 0),
			  accept(num)
		{}

		constexpr bool hasNext() { return (accept != 0 && parent::hasNext());}
		constexpr ItemType & getNext() {accept -= 1; return parent::getNext();}
	};
	#undef parent


	#define parentA Stream<StreamConcat<SourceTypeA, SourceTypeB, ItemType>, SourceTypeA, ItemType>
	template <typename SourceTypeA, typename SourceTypeB, typename ItemType>
		//requires (CoIterable<SourceTypeA, ItemType> && CoIterable<SourceTypeB, ItemType>)
	struct StreamConcat : public parentA {

		SourceTypeB source_b;
		bool a_has = true;

		constexpr StreamConcat(SourceTypeA &  a, SourceTypeB &  b, ItemType *) : parentA(a, 0), source_b(b) {}
		constexpr StreamConcat(SourceTypeA && a, SourceTypeB &  b, ItemType *) : parentA(a, 0), source_b(b) {}
		constexpr StreamConcat(SourceTypeA &  a, SourceTypeB && b, ItemType *) : parentA(a, 0), source_b(b) {}
		constexpr StreamConcat(SourceTypeA && a, SourceTypeB && b, ItemType *) : parentA(a, 0), source_b(b) {}

		constexpr bool hasNext() {
			if (a_has && parentA::hasNext())
				return true;
			else {
				a_has = false;
				return source_b.hasNext();
			}
		}
		constexpr ItemType & getNext() {
			if (a_has) return parentA::getNext();
			else       return source_b.getNext();
		}
		constexpr ItemType & getCurr()  {
			if (a_has) return parentA::getCurr();
			else       return source_b.getCurr();
		}
		constexpr void restore() { parentA::restore(); source_b.restore();}
		constexpr void prepare() { parentA::prepare(); source_b.prepare();}

	};
	#undef parentA

	namespace Iterators {
		template <typename Type>
		std::size_t GetSize(CoEnumeration<Type> auto & coll) {return coll.length;}

		template <typename Type>
		std::size_t GetSize(CoStdEnumeration<Type> auto & coll) {return coll.size();}
	}

	template <typename Coll, typename Type>
	struct DefaultIterator {
		Coll & coll;
		size_t index = (size_t)(-1);

		constexpr DefaultIterator(Coll & _coll) : coll(_coll) {}
		constexpr DefaultIterator(Coll & _coll, Type *) : coll(_coll) {}

		constexpr Type & getNext() {return coll[++index];};
		constexpr Type & getCurr() {return coll[index];};
		constexpr bool hasNext() {return Iterators::GetSize<Type>(coll) > (index + 1);}
		constexpr void restore() {index = (size_t)(-1);}
		constexpr void prepare() {}
		constexpr auto GetStream() {return StreamIterator(*this, (Type *)0);}
	};

	template <typename Type>
	struct PointerArrayIterator {
		Type * coll;
		size_t size;
		size_t index = (size_t)(-1);

		constexpr PointerArrayIterator(Type * _coll, size_t _size) : coll(_coll), size(_size) {}

		constexpr Type & getNext() {return coll[++index];};
		constexpr Type & getCurr() {return coll[index];};
		constexpr bool   hasNext() {return size > (index + 1);}
		constexpr void   restore() {index = (size_t)(-1);}
		constexpr void   prepare() {}
		constexpr auto GetStream() {return StreamIterator(*this, (Type *)0);}
	};

	template <typename Type>
	constexpr auto GetIterator(CoCandidateEnumeration<Type> auto & coll) {return DefaultIterator(coll, (Type *)0);}

	template <typename SourceType, typename ItemType>
	//	requires CoIterable<SourceType, ItemType>
	#define parent Stream<StreamIterator<SourceType, ItemType>, SourceType, ItemType>
	struct StreamIterator : public parent {
		constexpr StreamIterator(SourceType &  coll, ItemType *) : parent(coll, (ItemType *)0){}
		constexpr StreamIterator(SourceType && coll, ItemType *) : parent(coll, (ItemType *)0){}
	};
	#undef parent

	template <typename Type>
	constexpr auto GetStreamFromIterator(CoIterable<Type> auto & coll) {return StreamIterator(coll, (Type *)0);}
	template <typename Type>
	constexpr auto GetStreamFromIterator(CoIterable<Type> auto && coll) {return StreamIterator(coll, (Type *)0);}

	template <typename Type>
	constexpr auto GetStream(CoCandidateEnumeration<Type> auto & coll) {return StreamIterator(DefaultIterator(coll, (Type *)0), (Type *)0);}
	
	template <typename Type>
	constexpr auto GetStream(Type * pointer, size_t size) {return StreamIterator(PointerArrayIterator(pointer, size), (Type *)0);}
}
#endif
