#ifndef UF_T_Interface_H
#define UF_T_Interface_H

#include <functional>
#include "../Utils/Ok.h"

using namespace utils;

namespace uft {
	template<typename type>
	struct ICollection {
		virtual type & operator[] (size_t index) const = 0;
		virtual size_t size() const = 0;
		virtual size_t length() const = 0;
		virtual ICollection & foreach(std::function<void (type *)> lambda) = 0;
	};
	
	template<typename type>
	struct ICollectionAdvancedFunctions {
		virtual ICollectionAdvancedFunctions & addCopy    (type * obj) = 0;
		virtual ICollectionAdvancedFunctions & addCopy    (type   obj) = 0;
		virtual ICollectionAdvancedFunctions & addRef     (type * obj) = 0;
		virtual ICollectionAdvancedFunctions & addCopy    (size_t index, type * obj) = 0;
		virtual ICollectionAdvancedFunctions & addCopy    (size_t index, type   obj) = 0;
		virtual ICollectionAdvancedFunctions & addRef     (size_t index, type * obj) = 0;
		virtual ICollectionAdvancedFunctions & addAllCopy (size_t index, ICollection<type> & col) = 0;
		virtual ICollectionAdvancedFunctions & addAllRef  (size_t index, ICollection<type> & col) = 0;
		virtual ICollectionAdvancedFunctions & remove     (size_t index) = 0;
		virtual ICollectionAdvancedFunctions & removeFirst(const type * obj) = 0;
		virtual ICollectionAdvancedFunctions & removeLast (const type * obj) = 0;
		virtual ICollectionAdvancedFunctions & removeAll  (const type * obj) = 0;
		virtual ICollectionAdvancedFunctions & removeFirst() = 0;
		virtual ICollectionAdvancedFunctions & removeLast () = 0;
		virtual ICollectionAdvancedFunctions & removeAll  () = 0;
		virtual Ok<size_t> indexOf     (const type * obj) const = 0;
		virtual Ok<size_t> indexOfLast (const type * obj) const = 0;
	};

	//MB concept?
	template<typename type>
	struct ICopyble {
	public:
		virtual type copy() const = 0;
	};

	//MB use std realization?
	template<typename type>
	struct IIterible {
	public:
		virtual type * begin() = 0;
		virtual type * next() = 0;
		virtual type * end() = 0;
	};
	template<typename type>
	struct ISerializible {
	public:	
		virtual type serialize  (type to) = 0;
		virtual type deserialize(type from) = 0;
	};
	template<typename type>
	struct IComparable {
	public:		
		virtual bool equals(type object) = 0;
		virtual int compare(type object) = 0;
	};
	
}

#endif