#ifndef UF_U_Stalloc_H
#define UF_U_Stalloc_H
#if defined(UNIX)
#include <alloca.h>
#elif defined WIN32
#include <malloc.h>
#else 
//IDK what i should use here
#include <malloc.h>
#endif
//stack allocation. Free on leave function
#define stalloc(size, type) (type * )alloca((size) * sizeof(type))

#endif