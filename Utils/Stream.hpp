#ifndef UF_U_Stream_H
#define UF_U_Stream_H
#include "Concepts.hpp"
#include "Ok.hpp"
#include <utility>
#include <stdio.h>

//UseFull Utils Stream api module
//Version 1.0 alfa
//Make by Graph Don'te-Crypto

//#define STREAM_DEBUG

#include <UseFull/Utils/RuntimeDebug.hpp>

#ifdef RUNTIME_DEBUG
#include <cxxabi.h>
#endif // RUNTIME_DEBUG

namespace utils {

	template <typename RealType, typename SourceType, typename Lambda, typename TypeFrom, typename TypeTo>
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

	template <typename SourceType,  typename Lambda, typename TypeFrom, typename TypeTo>
		requires CoIterable<SourceType, TypeFrom>
	struct StreamIgnore;

	template <typename SourceType,  typename Lambda, typename TypeFrom, typename TypeTo>
		requires CoIterable<SourceType, TypeFrom>
	struct StreamAccept;

	template <typename SourceType,  typename Lambda, typename TypeFrom, typename TypeTo>
		requires CoIterable<SourceType, TypeFrom>
	struct StreamWhile;

	template <typename SourceType,  typename Lambda, typename TypeFrom, typename TypeTo>
		requires CoIterable<SourceType, TypeFrom>
	struct StreamSince;

	template <typename TypeFrom>
	auto GetStreamFromIterator(CoIterable<TypeFrom> auto & source);

	template<typename TypeFrom, typename TypeTo>
	TypeTo cmpstatic(TypeFrom, TypeTo *) {
		printf("CALL ZERO cmpstatic!!!\n");
		exit(1);
		return *((TypeTo *)0);
	}

	template<typename TypeFrom>
	TypeFrom cmpstatic(TypeFrom object, TypeFrom *){
		return object;
	}

	template <typename RealType, typename SourceType, typename Lambda, typename TypeFrom, typename TypeTo>
		requires CoIterable<SourceType, TypeFrom>
	struct Stream {

	    #define self (*((RealType *)this))

        DEBUG(
            const char * full_type;
            const char * type;
            const char * type_to;
            const char * source_type;
        )
		SourceType source;
		const Lambda lambda;
		TypeTo object;

		//Нужно делать через enable_if или я не знаю
		//Можно сделать наследников с переопределением функций hasNext и getNext
		bool hasNext() {
			return source.hasNext();
		}
		TypeTo & getNext() {
			object = cmpstatic(source.getNext(), (TypeTo *)0);
			return object;
		}

		TypeTo & getCurr()  {
			object = cmpstatic(source.getCurr(), (TypeTo *)0);
			return object;
		}

		void restore() { source.restore(); }

		Stream(const SourceType & _source, const Lambda & _lambda, TypeFrom *, TypeTo * f) :
			source(_source),
			lambda(_lambda),
			object()
		{
		    DEBUG(
                int status;
                source_type = abi::__cxa_demangle(typeid(_source).name(),0,0,&status);
                full_type = abi::__cxa_demangle(typeid(*this).name(),0,0,&status);
                type = "Stream";
                type_to = typeid(f).name();
            )
		}

		~Stream() {

		}

		template <typename NewType>
		auto Select(CoLambda<NewType, TypeTo> auto new_lambda) {
			return StreamSelect(*((RealType *)this), new_lambda, (TypeTo *)0, (NewType *)0);
		}

		auto ForAll(CoVoidLambda<TypeTo> auto new_lambda) {
			return StreamForAll(*((RealType *)this), new_lambda, (TypeTo *)0, (TypeTo *)0);
		}

		auto ForOne(CoVoidLambda<TypeTo> auto new_lambda) {
            return StreamForOne(*((RealType *)this), new_lambda, (TypeTo *)0, (TypeTo *)0);
		}

		auto Filter(CoLambda<bool, TypeTo> auto new_lambda) {
			return StreamFilter(*((RealType *)this), new_lambda, (TypeTo *)0, (TypeTo *)0);
		}

		auto While(CoLambda<bool, TypeTo> auto new_lambda) {
			return StreamWhile(*((RealType *)this), new_lambda, (TypeTo *)0, (TypeTo *)0);
		}

		auto Since(CoLambda<bool, TypeTo> auto new_lambda) {
			return StreamSince(*((RealType *)this), new_lambda, (TypeTo *)0, (TypeTo *)0);
		}

		TypeTo Reduce(CoLambda<TypeTo, TypeTo, TypeTo> auto new_lambda) {
			//Clang compatibility
			auto a = StreamSimpleReduce(*((RealType *)this), new_lambda, (TypeTo *)0, (TypeTo *)0);
			return a.reduce();
		}

		template <typename NewType>
		NewType Reduce(CoLambda<NewType, NewType, TypeTo> auto new_lambda, NewType & base) {
			//Clang compatibility
			auto a = StreamReduce(*((RealType *)this), new_lambda, (TypeTo *)0, base);
			return a.reduce();
		}

		auto Ignore(size_t num) {
			return StreamIgnore(*((RealType *)this), [](){}, (TypeTo *)0, (TypeTo *)0, num);
		}

		auto Accept(size_t num) {
			return StreamAccept(*((RealType *)this), [](){}, (TypeTo *)0, (TypeTo *)0, num);
		}

		TypeTo First() {
			return self.getNext();
		}
		TypeTo Last() {
			while (self.hasNext())
				self.getNext();
			return object;
		}
		Ok<TypeTo> Find(CoLambda<bool, TypeTo> auto new_lambda) {
		    while (self.hasNext())
                if (new_lambda(self.getNext()))
                    return self.getCurr();
            return {};
		}
	};

    #define parent(type) Stream< type <SourceType, Lambda, TypeFrom, TypeTo>, SourceType, Lambda, TypeFrom, TypeTo>

	template <typename SourceType, typename Lambda, typename TypeFrom, typename TypeTo>
		requires CoIterable<SourceType, TypeFrom>
	struct StreamSelect : public parent(StreamSelect) {
		StreamSelect(SourceType & _source, const Lambda & _lambda, TypeFrom *, TypeTo * f)
			: parent(StreamSelect)(_source, _lambda, (TypeFrom *)0, (TypeTo *)0)
		{

		    DEBUG(
                int status;
                parent(StreamSelect)::source_type = abi::__cxa_demangle(typeid(_source).name(),0,0,&status);
                parent(StreamSelect)::full_type = abi::__cxa_demangle(typeid(*this).name(),0,0,&status);
                parent(StreamSelect)::type = "StreamSelect";
                parent(StreamSelect)::type_to = typeid(f).name();
            )
		}

		bool hasNext() {
			return parent(StreamSelect)::hasNext();
		}
		TypeTo & getNext() {
			return (parent(StreamSelect)::object = parent(StreamSelect)::lambda(parent(StreamSelect)::source.getNext()));
		}

		TypeTo & getCurr() {
			return parent(StreamSelect)::object;
		}
	};

	template <typename SourceType, typename Lambda, typename TypeFrom, typename TypeTo>
		requires CoIterable<SourceType, TypeFrom>
	struct StreamForAll : public parent(StreamForAll) {

		StreamForAll(SourceType & _source, const Lambda & _lambda, TypeFrom *, TypeTo * f)
			: parent(StreamForAll)(_source, _lambda, (TypeFrom *)0, (TypeTo *)0)
		{

		    DEBUG(
                int status;
                parent(StreamForAll)::source_type = abi::__cxa_demangle(typeid(_source).name(),0,0,&status);
                parent(StreamForAll)::full_type = abi::__cxa_demangle(typeid(*this).name(),0,0,&status);
                parent(StreamForAll)::type = "StreamForAll";
                parent(StreamForAll)::type_to = typeid(f).name();
            )
			execute();
		}

		void execute() {
			while (parent(StreamForAll)::hasNext())
				parent(StreamForAll)::lambda(parent(StreamForAll)::getNext());
		}
	};
	template <typename SourceType, typename Lambda, typename TypeFrom, typename TypeTo>
		requires CoIterable<SourceType, TypeFrom>
	struct StreamForOne : public parent(StreamForOne) {
		StreamForOne(SourceType & _source, const Lambda & _lambda, TypeFrom *, TypeTo * f)
			: parent(StreamForOne)(_source, _lambda, (TypeFrom *)0, (TypeTo *)0)
		{

		    DEBUG(
                int status;
                parent(StreamForOne)::source_type = abi::__cxa_demangle(typeid(_source).name(),0,0,&status);
                parent(StreamForOne)::full_type = abi::__cxa_demangle(typeid(*this).name(),0,0,&status);
                parent(StreamForOne)::type = "StreamForOne";
                parent(StreamForOne)::type_to = typeid(f).name();
            )
		}

		bool hasNext() {
			return parent(StreamForOne)::hasNext();
		}
		TypeTo & getNext() {
			parent(StreamForOne)::getNext();
			parent(StreamForOne)::lambda(parent(StreamForOne)::object);
			return parent(StreamForOne)::object;
		}

		TypeTo & getCurr() {
			//maybe call lambda here
			return parent(StreamForOne)::object;
		}
	};

	template <typename SourceType, typename Lambda, typename TypeFrom, typename TypeTo>
		requires CoIterable<SourceType, TypeFrom>
	struct StreamFilter : public parent(StreamFilter) {

		bool nextGetted;

		StreamFilter(SourceType & _source, const Lambda & _lambda, TypeFrom *, TypeTo * f)
			: parent(StreamFilter)(_source, _lambda, (TypeFrom *)0, (TypeTo *)0),
			  nextGetted(true)
		{

		    DEBUG(
                int status;
                parent(StreamFilter)::source_type = abi::__cxa_demangle(typeid(_source).name(),0,0,&status);
                parent(StreamFilter)::full_type = abi::__cxa_demangle(typeid(*this).name(),0,0,&status);
                parent(StreamFilter)::type = "StreamFilter";
                parent(StreamFilter)::type_to = typeid(f).name();
            )
		}

		bool findNext() {
			while (parent(StreamFilter)::hasNext()) {
				parent(StreamFilter)::getNext();
				if (parent(StreamFilter)::lambda(parent(StreamFilter)::object)) {
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
			return parent(StreamFilter)::object;
		}

		TypeTo & getCurr() {
			nextGetted = true;
			return parent(StreamFilter)::object;
		}
	};

	template <typename SourceType, typename Lambda, typename TypeFrom, typename TypeTo>
		requires CoIterable<SourceType, TypeFrom>
	struct StreamWhile : public parent(StreamWhile) {

		bool nextGetted;
		bool active;

		StreamWhile(SourceType & _source, const Lambda & _lambda, TypeFrom *, TypeTo * f)
			: parent(StreamWhile)(_source, _lambda, (TypeFrom *)0, (TypeTo *)0),
			  nextGetted(true),
			  active(true)
		{
		    DEBUG(
                int status;
                parent(StreamWhile)::source_type = abi::__cxa_demangle(typeid(_source).name(),0,0,&status);
                parent(StreamWhile)::full_type = abi::__cxa_demangle(typeid(*this).name(),0,0,&status);
                parent(StreamWhile)::type = "StreamWhile";
                parent(StreamWhile)::type_to = typeid(f).name();
            )
		}

		bool findNext() {
			if (parent(StreamWhile)::hasNext()) {
				parent(StreamWhile)::getNext();
				active = parent(StreamWhile)::lambda(parent(StreamWhile)::object);
				nextGetted = false;
			}
			else active = false;
			return active;
		}

		bool hasNext() {
			if (active && !nextGetted) return true;
			else return findNext();
		}
		TypeTo & getNext() {
			nextGetted = true;
			return parent(StreamWhile)::object;
		}

		TypeTo & getCurr() {
			nextGetted = true;
			return parent(StreamWhile)::object;
		}
	};

	template <typename SourceType, typename Lambda, typename TypeFrom, typename TypeTo>
		requires CoIterable<SourceType, TypeFrom>
	struct StreamSince : public parent(StreamSince) {

		bool active;
		bool nextGetted;

		StreamSince(SourceType & _source, const Lambda & _lambda, TypeFrom *, TypeTo * f)
			: parent(StreamSince)(_source, _lambda, (TypeFrom *)0, (TypeTo *)0),
			  active(false),
			  nextGetted(true)
		{

		    DEBUG(
                int status;
                parent(StreamSince)::source_type = abi::__cxa_demangle(typeid(_source).name(),0,0,&status);
                parent(StreamSince)::full_type = abi::__cxa_demangle(typeid(*this).name(),0,0,&status);
                parent(StreamSince)::type = "StreamSince";
                parent(StreamSince)::type_to = typeid(f).name();
            )
		}

		bool findNext() {
			while (parent(StreamSince)::hasNext() && !active) {
				parent(StreamSince)::getNext();
				active = parent(StreamSince)::lambda(parent(StreamSince)::object);
			}
			nextGetted = !active;
			return active;
		}

		bool hasNext() {
			if (!active) return findNext();
			else return parent(StreamSince)::hasNext();
		}
		TypeTo & getNext() {
			if (!nextGetted) {
				nextGetted = true;
				return parent(StreamSince)::object;
			}
			return parent(StreamSince)::getNext();
		}

		TypeTo & getCurr() {
			return parent(StreamSince)::getCurr();
		}
	};

	template <typename SourceType, typename Lambda, typename TypeFrom, typename TypeTo>
		requires CoIterable<SourceType, TypeFrom>
	struct StreamReduce : public Stream<StreamReduce<SourceType, Lambda, TypeFrom, TypeTo>, SourceType, Lambda, TypeFrom, TypeFrom> {

		TypeTo result;

		StreamReduce(SourceType & _source, const Lambda & _lambda, TypeFrom *, TypeTo & base)
			: Stream<StreamReduce<SourceType, Lambda, TypeFrom, TypeTo>, SourceType, Lambda, TypeFrom, TypeFrom>(_source, _lambda, (TypeFrom *)0, (TypeFrom *)0),
			  result(base)
		{
            #define temp_parent Stream<StreamReduce<SourceType, Lambda, TypeFrom, TypeTo>, SourceType, Lambda, TypeFrom, TypeFrom>
		    DEBUG(
                int status;
                temp_parent::source_type = abi::__cxa_demangle(typeid(_source).name(),0,0,&status);
                temp_parent::full_type = abi::__cxa_demangle(typeid(*this).name(),0,0,&status);
                temp_parent::type = "StreamReduce";
                temp_parent::type_to = typeid(f).name();
            )
            #undef temp_parent
		}

		TypeTo & reduce() {
			while (Stream<StreamReduce<SourceType, Lambda, TypeFrom, TypeTo>, SourceType, Lambda, TypeFrom, TypeFrom>::hasNext())
				result = Stream<StreamReduce<SourceType, Lambda, TypeFrom, TypeTo>, SourceType, Lambda, TypeFrom, TypeFrom>::lambda(result, Stream<StreamReduce<SourceType, Lambda, TypeFrom, TypeTo>, SourceType, Lambda, TypeFrom, TypeFrom>::getNext());
			return result;
		}
	};

	template <typename SourceType, typename Lambda, typename TypeFrom, typename TypeTo>
		requires CoIterable<SourceType, TypeFrom>
	struct StreamSimpleReduce : public parent(StreamSimpleReduce) {

		TypeTo result;

		StreamSimpleReduce(SourceType & _source, const Lambda & _lambda, TypeFrom *, TypeTo * f)
			: parent(StreamSimpleReduce)(_source, _lambda, (TypeFrom *)0, (TypeTo *)0),
			  result()
		{
            DEBUG(
                int status;
                parent(StreamSimpleReduce)::source_type = abi::__cxa_demangle(typeid(_source).name(),0,0,&status);
                parent(StreamSimpleReduce)::full_type = abi::__cxa_demangle(typeid(*this).name(),0,0,&status);
                parent(StreamSimpleReduce)::type = "StreamSimpleReduce";
                parent(StreamSimpleReduce)::type_to = typeid(f).name();
            )
		}

		TypeTo & reduce() {
			while (parent(StreamSimpleReduce)::hasNext())
				result = parent(StreamSimpleReduce)::lambda(result, parent(StreamSimpleReduce)::getNext());
			return result;
		}
	};

	template <typename SourceType, typename Lambda, typename TypeFrom, typename TypeTo>
		requires CoIterable<SourceType, TypeFrom>
	struct StreamIgnore : public parent(StreamIgnore) {

		size_t ignore;

		StreamIgnore(SourceType & _source, const Lambda & _lambda, TypeFrom *, TypeTo *, size_t num)
			: parent(StreamIgnore)(_source, _lambda, (TypeFrom *)0, (TypeTo *)0),
			  ignore(num)
		{

		    DEBUG(
                int status;
                parent(StreamIgnore)::source_type = abi::__cxa_demangle(typeid(_source).name(),0,0,&status);
                parent(StreamIgnore)::full_type = abi::__cxa_demangle(typeid(*this).name(),0,0,&status);
                parent(StreamIgnore)::type = "StreamIgnore";
                parent(StreamIgnore)::type_to = typeid(f).name();
            )
		}

		bool findNext() {
			while (ignore != 0 && parent(StreamIgnore)::hasNext()) {
				parent(StreamIgnore)::getNext();
				ignore -= 1;
			}
			return (ignore == 0) && parent(StreamIgnore)::hasNext();
		}

		bool hasNext() {
			if (ignore != 0) return findNext();
			else return parent(StreamIgnore)::hasNext();
		}
		TypeTo & getNext() {
			if (ignore != 0) findNext();
			return parent(StreamIgnore)::getNext();
		}

		TypeTo & getCurr() {
			if (ignore != 0) findNext();
			return parent(StreamIgnore)::getCurr();
		}
	};

	template <typename SourceType,  typename Lambda, typename TypeFrom, typename TypeTo>
		requires CoIterable<SourceType, TypeFrom>
	struct StreamAccept : public parent(StreamAccept) {

		size_t accept;

		StreamAccept(SourceType & _source, const Lambda & _lambda, TypeFrom *, TypeTo * f, size_t num)
			: parent(StreamAccept)(_source, _lambda, (TypeFrom *)0, (TypeTo *)0),
			  accept(num)
		{

		    DEBUG(
                int status;
                parent(StreamAccept)::source_type = abi::__cxa_demangle(typeid(_source).name(),0,0,&status);
                parent(StreamAccept)::full_type = abi::__cxa_demangle(typeid(*this).name(),0,0,&status);
                parent(StreamAccept)::type = "StreamAccept";
                parent(StreamAccept)::type_to = typeid(f).name();
            )
		}

		bool hasNext() {
			if (accept) return parent(StreamAccept)::hasNext();
			return false;
		}
		TypeTo & getNext() {
			if (accept) accept -= 1;
			return parent(StreamAccept)::getNext();
		}

		TypeTo & getCurr() {
			return parent(StreamAccept)::getCurr();
		}

	};

	#undef parent

	namespace Iterators {
		template <typename Type>
		std::size_t GetSize(CoEnumeration<Type> auto & coll) {return coll.length;}

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
			return utils::GetStreamFromIterator<Type>(*this);
		}
	};

	template <typename Type>
	auto GetIterator(CoCandidateEnumeration<Type> auto & coll) {
		return Iterator(coll, (Type *)0);
	}

	template <typename Type>
	auto GetStreamFromIterator(CoIterable<Type> auto & source) {
		return Stream(source, [](){}, (Type *)0, (Type *)0);
	}

	template <typename SourceType, typename Lambda, typename TypeTo>
	struct StreamIterator : public Stream<StreamIterator<SourceType, Lambda, TypeTo>, Iterator<SourceType, TypeTo>, Lambda, TypeTo, TypeTo> {

        StreamIterator(SourceType & coll, const Lambda & _lambda, TypeTo *, TypeTo *) :
            //iterator(coll),
            Stream<StreamIterator<SourceType, Lambda, TypeTo>, Iterator<SourceType, TypeTo>, Lambda, TypeTo, TypeTo>(
                Iterator<SourceType, TypeTo>(coll),
                _lambda, (TypeTo *)0, (TypeTo *)0)
        {
            #define temp_parent Stream<StreamIterator<SourceType, Lambda, TypeTo>, Iterator<SourceType, TypeTo>, Lambda, TypeTo, TypeTo>

            DEBUG(
                int status;
                TypeTo a;
                temp_parent::source_type = abi::__cxa_demangle(typeid(_source).name(),0,0,&status);
                temp_parent::full_type = abi::__cxa_demangle(typeid(*this).name(),0,0,&status);
                temp_parent::type = "StreamIterator";
                temp_parent::type_to = typeid(a).name();
            )
            #undef temp_parent
        }
	};

	template <typename Type>
	auto GetStream(CoCandidateEnumeration<Type> auto & coll) {
        return StreamIterator(coll, [](){}, (Type *)0, (Type *)0);
	}
}
#endif
