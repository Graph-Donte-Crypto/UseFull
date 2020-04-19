#ifndef UF_U_Stalloc_H
#define UF_U_Stalloc_H

//stack allocation. Free on leave function
#define stalloc(size, type) (type * )alloca((size) * sizeof(type))

#endif