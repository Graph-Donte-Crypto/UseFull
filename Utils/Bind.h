#ifndef UF_U_Bind_H
#define UF_U_Bind_H

//UseFull Utils Bind module
//Version 1.0 alfa
//Make by Graph Don'te-Crypto

namespace utils {

	template <typename T>
	struct Bind {
		T * ptr;
		Bind()            {ptr =  nullptr;}
		Bind(T * bindptr) {ptr =  bindptr;}
		Bind(T & bindptr) {ptr = &bindptr;}
		inline void bind(T * bindptr) {ptr =  bindptr;}
		inline void bind(T & bindptr) {ptr = &bindptr;}
		inline operator T    &   () const {return          (*ptr);}
		template<typename type>
		inline operator type     () const {return   type   (*ptr);}
		template<typename type>
		inline          type & as() const {return *(type *)( ptr);}
	};
} 

#endif