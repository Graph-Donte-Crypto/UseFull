#ifndef UF_U_Prefix_H
#define UF_U_Prefix_H


namespace utils {
	#define prefix_stringify(something) #something
	#define prefix_unused(something) ((void)something)
	#define ptlu (unsigned long)(size_t)
}

//#define LOG_TRACE printf("Entering %s() (%s:%d)\n", __FUNCTION__, __FILE__, __LINE__);

#endif