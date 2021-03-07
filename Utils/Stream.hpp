#ifndef UF_U_Stream_H
#define UF_U_Stream_H

#include "Concepts.hpp"
#include <stdio.h>

//UseFull Utils Stream api module
//Version 1.0 alfa
//Make by Graph Don'te-Crypto

namespace utils {

	template <typename SourceType,  typename Lambda, typename TypeFrom, typename TypeTo>
		requires CoIterable<SourceType, TypeFrom>
	struct Stream;

	template <typename SourceType,  typename Lambda, typename TypeFrom, typename TypeTo>
		requires CoIterable<SourceType, TypeFrom>
	struct StreamSelect;

	template <typename SourceType,  typename Lambda, typename TypeFrom, typename TypeTo>
		requires CoIterable<SourceType, TypeFrom>
	struct StreamForAll;

	template <typename SourceType,  typename Lambda, typename TypeFrom, typename TypeTo>
		requires CoIterable<SourceType, TypeFrom>
	struct StreamForOne;

	template <typename SourceType,  typename Lambda, typename TypeFrom, typename TypeTo>
		requires CoIterable<SourceType, TypeFrom>
	struct StreamFilter;

	template <typename SourceType,  typename Lambda, typename TypeFrom, typename TypeTo>
		requires CoIterable<SourceType, TypeFrom>
	struct StreamSimpleReduce;

	template <typename SourceType,  typename Lambda, typename TypeFrom, typename TypeTo>
		requires CoIterable<SourceType, TypeFrom>
	struct StreamReduce;

	template <typename TypeFrom>
	auto GetStream(CoIterable<TypeFrom> auto & source);

	template<typename TypeFrom, typename TypeTo>
	TypeTo cmpstatic(TypeFrom, TypeTo *){
		printf("CALL ZERO cmpstatic!!!\n");
		return *((TypeTo *)0);
	}

	template<typename TypeFrom>
	TypeFrom cmpstatic(TypeFrom object, TypeFrom *){
		return object;
	}

	template <typename SourceType,  typename Lambda, typename TypeFrom, typename TypeTo>
		requires CoIterable<SourceType, TypeFrom>
	struct Stream {
		SourceType & source;
		int index = 0;
		const Lambda lambda;
		TypeTo object;
		
		//Нужно делать через enable_if или я не знаю
		//Можно сделать наследников с переопределением функций hasNext и getNext
		virtual bool hasNext() {
			return source.hasNext();
		}
		virtual TypeTo & getNext() {
			object = cmpstatic(source.getNext(), (TypeTo *)0);
			return object;
		}
		
		virtual TypeTo & getCurr()  {
			object = cmpstatic(source.getCurr(), (TypeTo *)0);
			return object;
		}
		
		void restore() { source.restore(); }
		
		Stream(SourceType & _source, const Lambda & _lambda, TypeFrom *, TypeTo *) : 
			source(_source), 
			lambda(_lambda), 
			object() 
		{}
		
		virtual ~Stream() {
			
		}
		
		template <typename NewType>
		auto Select(CoLambda<NewType, TypeTo> auto new_lambda) {
			return StreamSelect(*this, new_lambda, (TypeTo *)0, (NewType *)0);
		}
		
		auto ForAll(CoVoidLambda<TypeTo> auto new_lambda) {
			return StreamForAll(*this, new_lambda, (TypeTo *)0, (TypeTo *)0);
		}
		
		auto ForOne(CoVoidLambda<TypeTo> auto new_lambda) {
			return StreamForOne(*this, new_lambda, (TypeTo *)0, (TypeTo *)0);
		}
		
		auto Filter(CoLambda<bool, TypeTo> auto new_lambda) {
			return StreamFilter(*this, new_lambda, (TypeTo *)0, (TypeTo *)0);
		}
		
		TypeTo Reduce(CoLambda<TypeTo, TypeTo, TypeTo> auto new_lambda) {
			//Clang compatibility 
			auto a = StreamSimpleReduce(*this, new_lambda, (TypeTo *)0, (TypeTo *)0);
			return a.reduce();
		}
		
		template <typename NewType>
		NewType Reduce(CoLambda<NewType, NewType, TypeTo> auto new_lambda, NewType & base) {
			//Clang compatibility 
			auto a = StreamReduce(*this, new_lambda, (TypeTo *)0, base);
			return a.reduce();
		}
	};

	#define parent Stream<SourceType, Lambda, TypeFrom, TypeTo>

	template <typename SourceType,  typename Lambda, typename TypeFrom, typename TypeTo>
		requires CoIterable<SourceType, TypeFrom>
	struct StreamSelect : public Stream<SourceType, Lambda, TypeFrom, TypeTo> {
		StreamSelect(SourceType & _source, const Lambda & _lambda, TypeFrom *, TypeTo *) 
			: parent(_source, _lambda, (TypeFrom *)0, (TypeTo *)0)
		{}
		
		bool hasNext() override {
			return parent::hasNext();
		}
		TypeTo & getNext() override {
			return (parent::object = parent::lambda(parent::source.getNext()));
		}
		
		TypeTo & getCurr() override {
			return parent::object;
		}
	};

	template <typename SourceType,  typename Lambda, typename TypeFrom, typename TypeTo>
		requires CoIterable<SourceType, TypeFrom>
	struct StreamForAll : public Stream<SourceType, Lambda, TypeFrom, TypeTo> {
		
		StreamForAll(SourceType & _source, const Lambda & _lambda, TypeFrom *, TypeTo *) 
			: parent(_source, _lambda, (TypeFrom *)0, (TypeTo *)0) 
		{
			execute();
		}
		
		void execute() {
			while (parent::hasNext())
				parent::lambda(parent::getNext());
		}
	};

	template <typename SourceType,  typename Lambda, typename TypeFrom, typename TypeTo>
		requires CoIterable<SourceType, TypeFrom>
	struct StreamForOne : public Stream<SourceType, Lambda, TypeFrom, TypeTo> {
		StreamForOne(SourceType & _source, const Lambda & _lambda, TypeFrom *, TypeTo *) 
			: parent(_source, _lambda, (TypeFrom *)0, (TypeTo *)0)
		{}
		
		bool hasNext() {
			return parent::hasNext();
		}
		TypeTo & getNext() {
			parent::getNext();
			parent::lambda(parent::object);
			return parent::object;
		}
		
		TypeTo & getCurr() {
			//maybe call lambda here
			return parent::object;
		}
	};

	template <typename SourceType,  typename Lambda, typename TypeFrom, typename TypeTo>
		requires CoIterable<SourceType, TypeFrom>
	struct StreamFilter : public Stream<SourceType, Lambda, TypeFrom, TypeTo> {
		
		bool nextGetted;
		
		StreamFilter(SourceType & _source, const Lambda & _lambda, TypeFrom *, TypeTo *) 
			: parent(_source, _lambda, (TypeFrom *)0, (TypeTo *)0),
			  nextGetted(true)
		{}
		
		bool findNext() {
			while (parent::hasNext()) {
				parent::getNext();
				if (parent::lambda(parent::object)) {
					nextGetted = false;
					break;
				}
			}
			return !nextGetted;
		}
		
		bool hasNext() {
			if (!nextGetted) return true;
			else return findNext();
		}
		TypeTo & getNext() {
			nextGetted = true;
			return parent::object;
		}
		
		TypeTo & getCurr() {
			nextGetted = true;
			return parent::object;
		}
	};

	template <typename SourceType,  typename Lambda, typename TypeFrom, typename TypeTo>
		requires CoIterable<SourceType, TypeFrom>
	struct StreamReduce : public Stream<SourceType, Lambda, TypeFrom, TypeTo> {
		
		TypeTo result;
		
		StreamReduce(SourceType & _source, const Lambda & _lambda, TypeFrom *, TypeTo & base) 
			: parent(_source, _lambda, (TypeFrom *)0, (TypeTo *)0),
			  result(base)
		{}
		
		TypeTo & reduce() {
			while (parent::hasNext())
				result = parent::lambda(result, parent::getNext());
			return result;
		}
	};

	template <typename SourceType,  typename Lambda, typename TypeFrom, typename TypeTo>
		requires CoIterable<SourceType, TypeFrom>
	struct StreamSimpleReduce : public Stream<SourceType, Lambda, TypeFrom, TypeTo> {
		
		TypeTo result;
		
		StreamSimpleReduce(SourceType & _source, const Lambda & _lambda, TypeFrom *, TypeTo *) 
			: parent(_source, _lambda, (TypeFrom *)0, (TypeTo *)0),
			  result()
		{}
		
		TypeTo & reduce() {
			while (parent::hasNext())
				result = parent::lambda(result, parent::getNext());
			return result;
		}
	};

	#undef parent

	namespace Iterators {
		template <typename Type>
		std::size_t GetSize(CoEnumeration<Type> auto & coll) {return coll.size;}
		
		template <typename Type>
		std::size_t GetSize(CoStdEnumeration<Type> auto & coll) {return coll.size();}
	}

	template <typename Coll, typename Type>
	struct Iterator {
		Coll & coll;
		size_t index = (size_t)(-1);
		
		Iterator(Coll & _coll) : coll(_coll) {
			
		}
		
		Iterator(Coll & _coll, Type *) : coll(_coll) {
			
		}
		
		Type & getNext() {
			return coll[++index];
		};
		
		Type & getCurr() {
			return coll[index];
		};
		
		bool hasNext() {
			return Iterators::GetSize<Type>(coll) > (index + 1);
		}
		
		void restore() {
			index = (size_t)(-1);
		}
		
		auto GetStream() {
			return utils::GetStream<Type>(*this);
		}
	};

	template <typename Type>
	auto GetIterator(CoStdEnumeration<Type> auto & coll) {
		return Iterator(coll, (Type *)0);
	}

	template <typename Type>
	auto GetStream(CoIterable<Type> auto & source) {
		return Stream(source, [](){}, (Type *)0, (Type *)0);
	}
}
#endif 