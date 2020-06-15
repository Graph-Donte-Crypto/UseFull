#ifndef UF_U_Minmax_H
#define UF_U_Minmax_H

//UseFull Min and Max module
//Version 1.0 alfa
//Make by Graph Don'te-Crypto

namespace utils {
	template <typename Type>
	inline Type min(Type a, Type b) {return a < b ? a : b;}
	
	template <typename Type>
	inline Type min(Type a, Type b, Type c) {return min(a, min(b, c));}
	
	template <typename Type>
	inline Type min(Type a, Type b, Type c, Type d) {return min(a, min(b, c, d));}
	
	template <typename Type>
	inline Type min(Type a, Type b, Type c, Type d, Type e) {return min(a, min(b, c, d, e));}
	
	
	template <typename Type>
	inline Type max(Type a, Type b) {return a > b ? a : b;}
	
	template <typename Type>
	inline Type max(Type a, Type b, Type c) {return max(a, max(b, c));}
	
	template <typename Type>
	inline Type max(Type a, Type b, Type c, Type d) {return max(a, max(b, c, d));}
	
	template <typename Type>
	inline Type max(Type a, Type b, Type c, Type d, Type e) {return max(a, max(b, c, d, e));}
	
	template <typename Type>
	inline void minmax(Type a, Type b, Type & ret_a, Type & ret_b) {
		if (a < b) {ret_a = a; ret_b = b;} else {ret_a = b; ret_b = a;};
	}
}

#endif