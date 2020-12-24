#ifndef UF_U_Stalloc_H
#define UF_U_Stalloc_H

#include <stdio.h>      // vsnprintf, sscanf, printf
#if !defined(alloca)
	#if defined(__GLIBC__) || defined(__sun) || defined(__CYGWIN__)
		#include <alloca.h>     // alloca
	#elif defined(_WIN32)
		#include <malloc.h>     // alloca
		#if !defined(alloca)
			#define alloca _alloca  // for clang with MS Codegen
		#endif
	#else
		#include <stdlib.h>     // alloca
	#endif
#endif
//stack allocation. Free on leave function
#define stalloc(size, type) (type * )alloca((size) * sizeof(type))

#endif