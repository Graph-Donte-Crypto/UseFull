#ifndef UF_U_Macro_H
#define UF_U_Macro_H

namespace utils {
	#define prefix_stringify(something) #something
	#define prefix_unused(something) ((void)something)
	#define ptlu (unsigned long)(unsigned long long)
	#define std_cast(TYPE, OBJ) ((TYPE)(OBJ))
	#define ptr_cast(TYPE, OBJ) (*(TYPE *)(&OBJ))
}

#endif