#ifndef UF_U_Bytes_H
#define UF_U_Bytes_H

#include <string.h> 

//UseFull Utils Bytes module
//Version 1.0 alfa
//Make by Graph Don'te-Crypto

namespace utils {
	char * toBytes(char * out, void * in, size_t size) {
	    memcpy((void *)out, in, size);
	    return out + size;
	}
	template <typename T>
	char * toBytes(char * out, T in, size_t size = sizeof(T)) {
	    memcpy((void *)out, &in, size);
	    return out + size;
	}

	char * fromBytes(void * out, char * in, size_t size) {
	    memcpy(out, (void *)in, size);
	    return in + size;
	}
	
}

#endif