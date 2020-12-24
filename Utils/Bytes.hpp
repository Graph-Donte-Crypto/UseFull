#ifndef UF_U_Bytes_H
#define UF_U_Bytes_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

//UseFull Utils Bytes module
//Version 1.0 alfa
//Make by Graph Don'te-Crypto

namespace utils {

	#define ptr_sum(a, b) ((void *)(((char *)(a)) + ((size_t)(b))))
	
	void * toBytes(void * out, const void * in, size_t size) {
		memcpy((void *)out, in, size);
		return ptr_sum(out, size);
	}
	template <typename T>
	void * toBytesObject(void * out, const T & in, size_t size = sizeof(T)) {
		memcpy((void *)out, &in, size);
		return ptr_sum(out, size);
	}

	const void * fromBytes(void * out, const void * in, size_t size) {
		memcpy(out, in, size);
		return ptr_sum(in, size);
	}
	
	
	void * shiftBytes(void * where, long long shift, size_t length_in_bytes) {
		memmove(ptr_sum(where, shift), where, length_in_bytes);
		return ptr_sum(where, shift);
	}
	
	template <typename T>
	T * shiftObjects(T * where, long long shift, size_t length_in_objects) {
		memmove(ptr_sum(where, shift), where, length_in_objects * sizeof(T));
		return ptr_sum(where, shift);
		/*
			//aaaaBaaaC___
			//B 3 5
			//aaaa___BaaaC
			
			//aaaaBaaaC__
			//B 3 4
			//aaaa___Baaa
			
			//aaaaBaaaa
			//B -3 5
			//aBaaaa???
		*/
	}
	
	struct Bits8 {
		unsigned char bit0 : 1;
		unsigned char bit1 : 1;
		unsigned char bit2 : 1;
		unsigned char bit3 : 1;
		unsigned char bit4 : 1;
		unsigned char bit5 : 1;
		unsigned char bit6 : 1;
		unsigned char bit7 : 1;
		template <typename T>
			requires (sizeof(T) == 1)
		Bits8 & setByte(T t) {
			memcpy(this, &t, 1);
			return *this;
		}
		template <typename T>
			requires (sizeof(T) == 1)
		Bits8 & operator = (T t) {
			return setByte(t);
		}
		template <typename T>
			requires (sizeof(T) != 1)
		Bits8 & operator = (T t) {
			return setByte((unsigned char)t);
		}
		unsigned char getByte() {
			char ret;
			memcpy(&ret, this, 1);
			return ret;
		}
		unsigned char operator[] (unsigned short i) {
			switch(i) {
				case 0: return bit0;
				case 1: return bit1;
				case 2: return bit2;
				case 3: return bit3;
				case 4: return bit4;
				case 5: return bit5;
				case 6: return bit6;
				case 7: return bit7;
				default:
					printf("Bits8::operator[]::i(%u) > 7\n", i);
					exit(1);
			}
		}
		void setBit(unsigned short i, bool value) {
			switch(i) {
				case 0: bit0 = value; break;
				case 1: bit1 = value; break;
				case 2: bit2 = value; break;
				case 3: bit3 = value; break;
				case 4: bit4 = value; break;
				case 5: bit5 = value; break;
				case 6: bit6 = value; break;
				case 7: bit7 = value; break;
				default:
					printf("Bits8::setBit::i(%u) > 7\n", i);
					exit(1);
			}
		}
	};
}

#endif