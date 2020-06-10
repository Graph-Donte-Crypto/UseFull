#ifndef UF_T_Array_H
#define UF_T_Array_H

#include "Interface.hpp"

//UseFull Template Array module
//Version 2.0 alfa
//Make by Graph Don'te-Crypto

namespace uft {
	using namespace utils;
	
	template <typename Type>
	struct Array {
		/*   Array all fields   */
		size_t _length = 0;
		size_t _size = 0;
		Type * values = nullptr;
		/* */
		
		/*   CoSerializible  */
		size_t getDataSize() {return _length * sizeof(Type);}
		void * packData  (void * ptr) {return toBytes(ptr, (void *)values, getDataSize());}
		const void * unpackData(const void * ptr, size_t size) {return fromBytes(values, ptr, size);}
		/* */
		
		/*   CoAllocatable   */
		void allocMemory(size_t size) {_size = size; values = new Type[size];}
		void freeMemory() {_size = 0; delete[] values;}
		/* */
		
		/*   CoCollection   */
		Type & operator[] (size_t index) const {return values[index];}
		size_t size() const {return _size;}
		size_t length() const {return _length;}
		void addCopy (size_t index, const Type * obj) {
			CollectionV2.checkSizeWithKeepValues(*this, _length + 1);
			_length += 1;
			addCopyUnsafe(index, obj);
		}
		void addCopy (              const Type * obj) {
			CollectionV2.checkSizeWithKeepValues(*this, _length + 1);
			_length += 1;
			addCopyUnsafe(_length - 1, obj);
		}
		void remove(size_t index) {
			//Object with [index + 1] shift to position [index], and other objects after
			shiftObjects(values + index + 1, -1, _length - index - 1);
			_length -= 1;
		};
		void removeAll() {_length = 0;}
		/* */
		
		/*   Array functions   */
		Array(size_t size) {allocMemory(size);}
		Array(const std::initializer_list<Type> & list) {
			allocMemory(list.size());
			fromBytes(values, list.begin(), sizeof(Type) * _size);
			_length = _size;
		}
		Array() {allocMemory(256);}
		~Array() {freeMemory();}
		
		size_t getElementPositionUnsafe(const Type * obj) {
			return (values - obj) / sizeof(Type);
		}
		Ok<size_t> getElementPosition(const Type * obj) {
			if (obj < values) return {};
			if (obj > values + length * sizeof(Type)) return {};
			return getElementPositionUnsafe(obj);
		}
		void addCopyUnsafe(size_t index, const Type * obj) {*(values + index) = *obj;}	
		void addAllCopy (const std::initializer_list<Type> & list) {
			CollectionV2.checkSizeWithKeepValues(*this, _length + list.size());
			fromBytes(values + _length, list.begin(), sizeof(Type) * list.size());
			_length += list.size();
		}
		/* */
		
		/*   CollectionFunctions   */
		void addCopy(size_t index, const Type & obj) {CollectionFunctions<Type, Array<Type>>::addCopy(*this, index, obj);}
		void addCopy(const Type & obj) {CollectionFunctions<Type, Array<Type>>::addCopy(*this, obj);}
		void foreach(CoLambda<void, Type *> auto lambda) {CollectionFunctions<Type, Array<Type>>::foreach(*this, lambda);}
		void removeFirst() {CollectionFunctions<Type, Array<Type>>::removeFirst(*this);}
		void removeLast () {CollectionFunctions<Type, Array<Type>>::removeLast(*this);}
		Ok<size_t> indexByCondition(CoLambda<bool, Type *> auto lambda) {return CollectionFunctions<Type, Array<Type>>::indexByCondition(*this, lambda);}
		/* */
		
		/*   CollectionFunctions unstandart realisation   */
		void addAllCopy (CoCollection<Type> auto & inputColl) {
			CollectionV2.checkSizeWithKeepValues(*this, _length + inputColl.length());
			for (size_t i = 0; i < inputColl.length(); i++) 
				addCopyUnsafe(_length++, inputColl[i]);
		}
		void addAllCopy (size_t index, CoCollection<Type> auto & inputColl) {
			CollectionV2.checkSizeWithKeepValues(*this, _length + inputColl.length());
			shiftObjects(values + index, inputColl.length(), _length - index - 1);
			for (size_t i = 0; i < inputColl.length(); i++) 
				addCopyUnsafe(index++, inputColl[i]);
			_length += inputColl.length();
		}
		/* */	
	};
}

#endif
