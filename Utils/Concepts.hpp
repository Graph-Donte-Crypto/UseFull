#ifndef UF_U_Concepts_H
#define UF_U_Concepts_H

#include <concepts>

//UseFull Utils Concept module
//Version 1.1 alfa
//Make by Graph Don'te-Crypto

namespace utils {

	struct Nothing {
		Nothing(void){}
	};

	template <typename FUNC, typename RET, typename ... INPUT>
	concept CoLambda =
	requires (FUNC func, INPUT ... input) {
		{ func(input...) } -> std::same_as<RET>;
	};

	template <typename FUNC, typename ... INPUT>
	concept CoVoidLambda =
	requires (FUNC func, INPUT ... input) {
		{ func(input...) } -> std::same_as<void>;
	};

	/*
	Example:
		int function(CoLambda<int, Nothing> auto lambda) {
			return lambda({}) + 1;
		}
		...
		int a2 = function([](Nothing){return 2;});
	*/

	template <typename Type>
	concept CoSerializible =
	requires (Type obj, void * out, const void * in, std::size_t size) {
		{ obj.getDataSize() } -> std::same_as<std::size_t>;
		{ obj.packData(out) } -> std::same_as<void *>;
		{ obj.unpackData(in, size) } -> std::same_as<const void *>;
	};

	template <typename Type>
	concept CoAllocatable =
	requires (Type obj, std::size_t size, void * ptr) {
		{ obj.allocMemory(size) } -> std::same_as<void>;
		{ obj.freeMemory()      } -> std::same_as<void>;
		{ obj.freeMemory(ptr)   } -> std::same_as<void>;
		{ obj.setMemory(ptr)    } -> std::same_as<void>;
		{ obj.getMemory()       } -> std::same_as<void *>;
		{ obj.takeMemory()      } -> std::same_as<void *>;
		{ obj.getMemorySize()   } -> std::convertible_to<std::size_t>;
		{ obj.getDataSize()     } -> std::convertible_to<std::size_t>;
	};

	template <typename Coll, typename Type>
	concept CoIterable =
	requires (Coll coll, std::size_t size) {
		{ coll.hasNext() } -> std::same_as<bool>;
		{ coll.getNext() } -> std::convertible_to<Type>;
		{ coll.getCurr() } -> std::convertible_to<Type>;
		{ coll.restore() } -> std::same_as<void>;
	};

	template <typename TypeFrom, typename TypeTo>
	concept CoConvertible =
	requires (TypeFrom object) {
		{ object } -> std::convertible_to<TypeTo>;
	};

	template <typename Type>
	concept CoSizeable =
	requires (Type obj) {
		{ obj.size } -> std::convertible_to<std::size_t>;
	};

	template <typename Coll, typename Type>
	concept CoEnumeration  =
	requires (Coll coll, std::size_t index) {
		{ coll[index] } -> std::same_as<Type &>;
		{ coll.length } -> std::convertible_to<std::size_t>;
	};

	template <typename Coll, typename Type>
	concept CoCandidateEnumeration  =
	requires (Coll coll, std::size_t index) {
		{ coll[index] } -> std::same_as<Type &>;
	};

	template <typename Coll, typename Type>
	concept CoStdEnumeration  =
	requires (Coll coll, std::size_t index) {
		{ coll[index] } -> std::same_as<Type &>;
		{ coll.size() } -> std::convertible_to<std::size_t>;
	};

	template <typename Coll, typename Type>
	concept CoCollection  =
	requires (Coll coll, std::size_t index, const Type * obj) {
		{ coll[index] } -> std::same_as<Type &>;
		{ coll.size   } -> std::convertible_to<std::size_t>;
		{ coll.length } -> std::convertible_to<std::size_t>;
		{ coll.addCopy(index, obj) } -> std::same_as<void>;
		{ coll.addCopy(       obj) } -> std::same_as<void>;
		{ coll.remove(index)       } -> std::same_as<void>;
		{ coll.removeAll()         } -> std::same_as<void>;
	};
}

#endif
