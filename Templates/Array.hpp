#ifndef UF_T_Array_H
#define UF_T_Array_H

#include "../Utils/Bytes.hpp"
#include "Interface.hpp"

//UseFull Template Array module
//Version 2.0 alfa
//Make by Graph Don'te-Crypto

namespace uft {
	using namespace utils;

	template <typename Type>
	struct Array : public CollectionFunctions2<Array<Type>, Type>{
		/*   Array all fields   */
		Type * values = nullptr;
		/* */

		/*   CoSerializible  */
		size_t getDataSize() {return length * sizeof(Type);}
		void * packData  (void * ptr) {
			return toBytes(ptr, (void *)values, getDataSize());
		}
		const void * unpackData(const void * ptr, size_t size) {
			length = size / sizeof(Type);
			return fromBytes(values, ptr, size);
		}
		/* */

		/*   CoAllocatable   */
		void allocMemory(size_t size) {
			this->size = size; 
			length = 0; 
			values = (Type *) (new char [sizeof(Type) * size] );
		}
		void freeMemory() {size = 0; length = 0; delete[] values;}
		/* */

		void resizeMemoryWithFlush(size_t size) {
			freeMemory();
			allocMemory(size);
		}

		/*   CoCollection   */
		size_t length = 0;
		size_t size = 0;
		Type & operator[] (size_t index) const {return values[index];}
		Type & last() {return values[length - 1];}
		void addCopy (size_t index, const Type * obj) {
			CollectionV2.checkSizeWithKeepValues(*this, length + 1);
			length += 1;
			//Object with [index] shift to position [index + 1], and other objects after
			shiftObjects(values + index, 1, length - index);
			addCopyUnsafe(index, obj);
		}
		void addCopy (              const Type * obj) {
			CollectionV2.checkSizeWithKeepValues(*this, length + 1);
			length += 1;
			addCopyUnsafe(length - 1, obj);
		}
		void remove(size_t index) {
			//Object with [index + 1] shift to position [index], and other objects after
			shiftObjects(values + index + 1, -1, length - index - 1);
			length -= 1;
		};
		void removeAll() {length = 0;}
		/* */

		/*   Array functions   */

		Array(CoEnumeration<Type> auto enumeration) {
			allocMemory(enumeration.length);
			for (size_t i = 0; i < enumeration.length; i++)
				addCopyUnsafe(i, &enumeration[i]);
			length = enumeration.length;
		}

		Array(size_t size) {allocMemory(size);}
		Array(const std::initializer_list<Type> & list) {
			allocMemory(list.size());
			fromBytes(values, list.begin(), sizeof(Type) * size);
			length = size;
		}
		Array() {allocMemory(256);}
		~Array() {freeMemory();}

		Array & flushAndCopyFrom(const CoEnumeration<Type> auto & enumeration) {
		    CollectionV2.checkSizeWithoutKeepValues(*this, enumeration.length);
			for (size_t i = 0; i < enumeration.length; i++)
				addCopyUnsafe(i, &enumeration[i]);
			length = enumeration.length;
			return *this;
		}

    private:
        //защита от случайного прямого копирования
        Array & operator = (const Array & array) {
            return *this;
        }

    public:

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
			CollectionV2.checkSizeWithKeepValues(*this, length + list.size());
			fromBytes(values + length, list.begin(), sizeof(Type) * list.size());
			length += list.size();
		}
		void addAllCopy (const Type * objects, size_t obj_length) {
			CollectionV2.checkSizeWithKeepValues(*this, length + obj_length);
			fromBytes(values + length, objects, sizeof(Type) * obj_length);
			length += obj_length;
		}
		void removeByEquation(const Type & obj) {
			Ok<size_t> index = indexByEquation(obj);
			if (index.isOk) remove(index);
		}
		void removeAll(CoLambda<void, Type *> auto lambda) {
			foreach(lambda);
			length = 0;
		}
		/* */

		/*   CollectionFunctions   */
		void addCopy(size_t index, const Type & obj) {CollectionFunctions2<Array<Type>, Type>::addCopy(index, obj);}
		void addCopy(const Type & obj) {CollectionFunctions2<Array<Type>, Type>::addCopy(obj);}
		void foreach(CoLambda<void, Type *> auto lambda) {CollectionFunctions2<Array<Type>, Type>::foreach(lambda);}
		void foreach(CoLambda<void, Type &> auto lambda) {CollectionFunctions2<Array<Type>, Type>::foreach(lambda);}
		void removeFirst() {CollectionFunctions2<Array<Type>, Type>::removeFirst();}
		void removeLast () {CollectionFunctions2<Array<Type>, Type>::removeLast();}
		Ok<size_t> indexByCondition(CoLambda<bool, Type *> auto lambda) {return CollectionFunctions2<Array<Type>, Type>::indexByCondition(lambda);}
		Ok<size_t> indexByEquation(const Type & obj) {return CollectionFunctions2<Array<Type>, Type>::indexByEquation(obj);}
		/* */

		/*   CollectionFunctions nonstandard realization   */
		void addAllCopy (CoCollection<Type> auto & inputColl) {
			CollectionV2.checkSizeWithKeepValues(*this, length + inputColl.length);
			for (size_t i = 0; i < inputColl.length; i++)
				addCopyUnsafe(length++, &inputColl[i]);
		}
		void addAllCopy (size_t index, CoCollection<Type> auto & inputColl) {
			CollectionV2.checkSizeWithKeepValues(*this, length + inputColl.length);
			//Object with [index] shift to position [index + inputColl.length()], and other objects after
			shiftObjects(values + index, inputColl.length, length - index);
			for (size_t i = 0; i < inputColl.length; i++)
				addCopyUnsafe(index++, inputColl[i]);
			length += inputColl.length;
		}
		/* */
	};
}

#endif
