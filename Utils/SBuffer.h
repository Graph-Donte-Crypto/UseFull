#ifndef UF_U_SBuffer_H
#define UF_U_SBuffer_H

#include <string.h>

//UseFull Utils Stack Buffer module
//Version 1.0 alfa
//Make by Graph Don'te-Crypto

namespace utils {
	template <typename Type, size_t size>
	struct SBuffer {
		Type ptr[size];
		
		Type & operator [] (size_t i)       {return ptr[i];}
		Type   operator [] (size_t i) const {return ptr[i];}
		
		bool operator == (const SBuffer<Type, size> & sb) const {
			return !(memncmp(ptr, sb.ptr, size));
		}
		bool operator != (const SBuffer<Type, size> & sb) const {
			return memncmp(ptr, sb.ptr, size);
		}
		
		SBuffer& operator = (const SBuffer<Type, size> & buf) {
			memcpy(ptr, buf.ptr, size * sizeof(Type));
			return *this;
		}
		
	};
}

#endif