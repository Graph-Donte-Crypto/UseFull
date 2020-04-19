#ifndef UF_T_Array_H
#define UF_T_Array_H

#include "Interface.h"

//UseFull Template Array module
//Version 1.0 alfa
//Make by Graph Don'te-Crypto

namespace uft {
	template <typename type>
	struct Array : public ICollection<type> {
		size_t length = 0;
		size_t size = 0;
		type * values = nullptr;
		
		size_t iter = 0;
		size_t iter_back = 0;
		
		void sizeUp(size_t new_size) {
			if (new_size >= size) {
				do {
					size *= 2;
				} while (new_size > size);

				type * buf = new type[size];
				memcpy(buf, values, sizeof(type) * length);
				delete[] values;
				values = buf;
			}
		}
		void sizeUpNoCopy() {
			if (new_size >= size) {
				do {
					size *= 2;
				} while (new_size > size);
				delete[] values;
				type * buf = new type[size];
				values = buf;
			}
		}
		
		Array() {
			size = 1;
			sizeUp(256);
		}
		
		virtual type & operator[] (size_t index) const {return values[index];}
		virtual size_t size() const {return size;}
		virtual size_t length() const {return length;}
		virtual Array<type> & foreach(std::function<void (type *)> lambda) {
			for (iter = 0; iter < length; iter++) {
				iter_back = 0;
				lambda(values + iter);
				iter -= iter_back;
			}
		}
		
		virtual Array<type> & addCopy    (type * obj) {
			
		}
		virtual Array<type> & addCopy    (type   obj) {return addCopy(&obj);}
		virtual Array<type> & addCopy    (size_t index, type * obj) {
			
		}
		virtual Array<type> & addCopy    (size_t index, type   obj) {return addCopy(index, &obj);}
		virtual Array<type> & addAllCopy (size_t index, ICollection<type> & col) {
			
		}
		virtual Array<type> & remove     (size_t index) {
			if 
		};
		virtual Array<type> & removeFirst(const type * obj) = 0;
		virtual Array<type> & removeLast (const type * obj) = 0;
		virtual Array<type> & removeAll  (const type * obj) {}
		virtual Array<type> & removeFirst() {
			if (iter > 0) iter_back += 1;
			length -= 1; 
			memmove(values, values + 1, sizeof(type) * length);
		}
		virtual Array<type> & removeLast () {
			length -= 1;
		}
		virtual Array<type> & removeAll  () {
			length = 0;
		};
		virtual Ok<size_t> indexOf     (const type * obj) const = 0;
		virtual Ok<size_t> indexOfLast (const type * obj) const = 0;
		
	}
}

#endif
