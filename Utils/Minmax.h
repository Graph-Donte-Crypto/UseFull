#ifndef UF_U_Minmax_H
#define UF_U_Minmax_H

//UseFull Min and Max module
//Version 1.0 alfa
//Make by Graph Don'te-Crypto

namespace utils {
	template <typename type>
	inline type min(type a, type b) {return a < b ? a : b;}
	
	template <typename type>
	inline type min(type a, type b, type c) {return min(a, min(b, c));}
	
	template <typename type>
	inline type min(type a, type b, type c, type d) {return min(a, min(b, c, d));}
	
	template <typename type>
	inline type min(type a, type b, type c, type d, type e) {return min(a, min(b, c, d, e));}
	
	
	template <typename type>
	inline type max(type a, type b) {return a > b ? a : b;}
	
	template <typename type>
	inline type max(type a, type b, type c) {return max(a, max(b, c));}
	
	template <typename type>
	inline type max(type a, type b, type c, type d) {return max(a, max(b, c, d));}
	
	template <typename type>
	inline type max(type a, type b, type c, type d, type e) {return max(a, max(b, c, d, e));}
	
	template <typename type>
	inline void minmax(type a, type b, type & ret_a, type & ret_b) {
		if (a < b) {ret_a = a; ret_b = b;} else {ret_a = b; ret_b = a;};
	}
}

#endif