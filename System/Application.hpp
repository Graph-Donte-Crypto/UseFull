#ifndef UF_SY_Application_H
#define UF_SY_Application_H

//UseFull System Application module
//Version 1.0 alfa
//Make by Graph Don'te-Crypto

namespace ufsys {
	
	#ifdef WIN32
	
	struct ApplicationStruct {
		char prog_dir[MAX_PATH];
		char curr_dir[MAX_PATH];
		
		ApplicationStruct() {
			prog_dir[0] = 0;
			curr_dir[0] = 0;
		}
		
		const char * ProgramDirectory() {
			if (prog_dir[0] == 0) {
				GetModuleFileNameA(nullptr, prog_dir, MAX_PATH);
				PathRemoveFileSpecA(prog_dir);
				PathAddBackslashA(prog_dir);
			}
			return prog_dir;
		}
		const char * CurrentDirectory() {
			GetCurrentDirectory(sizeof(curr_dir), curr_dir);
			return curr_dir;
		}
	}
	Application;
	
	#else 
	
	struct ApplicationStruct {
		char prog_dir[1024];
		char curr_dir[1024];
		
		ApplicationStruct() {
			prog_dir[0] = 0;
			curr_dir[0] = 0;
		}
		
		const char * ProgramDirectory();
		const char * CurrentDirectory();
	}
	Application;
	
	#endif
}

#endif