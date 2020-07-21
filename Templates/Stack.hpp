#ifndef UF_T_Stack_H
#define UF_T_Stack_H

#include "Array.hpp"

//UseFull Template Stack module
//Version 1.0 alfa
//Make by Graph Don'te-Crypto

namespace uft {
	template <typename Type>
	struct Stack {
		Array<Type> values = Array<Type>(4);
		void pushCopy(const Type & type) {
			values.addCopy(type);
		}
		Type & get() {
			return values[values.length - 1];
		}
		void pop() {
			values.length -= 1;
		}
	};
	
}

#endif