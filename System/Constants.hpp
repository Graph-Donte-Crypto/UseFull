#ifndef UF_SY_Constants_H
#define UF_SY_Constants_H

//UseFull System Constants module
//Version 1.0 alfa
//Make by Graph Don'te-Crypto

namespace ufsys {
	#ifdef WIN32
	constexpr const char * PathSplitter = "\\";
	#else 
	constexpr const char * PathSplitter = "/";
	#endif
}

#endif