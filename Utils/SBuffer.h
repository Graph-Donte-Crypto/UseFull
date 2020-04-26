#ifndef UF_U_SBuffer_H
#define UF_U_SBuffer_H

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
			for (size_t i = 0; i < size; i++) {
				if (ptr[i] != sb[i]) return false;
			}
			return true;
		}
		bool operator != (const SBuffer<Type, size> & sb) const {
			for (size_t i = 0; i < size; i++) {
				if (ptr[i] == sb[i]) return false;
			}
			return true;
		}
		
	};
}

#endif