#ifndef UF_SY_File_H
#define UF_SY_File_H

#if defined(WIN32)
#include <shlwapi.h>
#elif defined(UNIX)
#include <sys/stat.h>
#endif

#include <stdio.h>
#include <string.h>

#include "../Utils/Ok.hpp"
#include "../String/BaseLib.hpp"
#include "../Templates/Span.hpp"

#include "Constants.hpp"

//UseFull System File module
//Version 1.0 alfa
//Make by Graph Don'te-Crypto

namespace ufsys {
	struct File {
		const char * path = nullptr;
		const char * filename = nullptr;
		
		bool win_format = false;
		
		size_t size = 0;
		size_t rows = 0;
		char * bytes = nullptr;
		
		File & open(); 
				
		static constexpr const char LF = '\n';
		static constexpr const char CR = '\r';
		static constexpr const char * CRLF = "\r\n";
		
		static utils::Ok<File> InDirectory(const char * path, const char * filename);
		
	};	
	
	#if defined(WIN32)
	
	File & File::open() {	
		char fpath[MAX_PATH];
		strcpy(fpath, path);
		strcat(fpath, PathSplitter);
		strcat(fpath, filename);
	
		FILE * f = fopen(fpath, "rb");
		fseek(f, 0, SEEK_END);
		size = ftello64(f) + 1;
		fseek(f, 0, SEEK_SET);
		bytes = new char[size];
		fread(bytes, 1, size - 1, f);
		bytes[size - 1] = 0;
		if (strstr(bytes, CRLF) != nullptr) {
			win_format = true;
			rows = str::CountSubstr(bytes, CRLF);
		}
		else {
			rows = str::CountChar(bytes, LF);
		}
		fclose(f);
		return *this;	
	}
	
	utils::Ok<File> File::InDirectory(const char * path, const char * filename) {
		char fpath[MAX_PATH];
		strcpy(fpath, path);
		strcat(fpath, PathSplitter);
		strcat(fpath, filename);
		if (PathFileExistsA(fpath)) {
			File file;
			file.path = path;
			file.filename = filename;
			return file;
		}
		else return {};
	}
	
	#elif defined(UNIX)
	utils::Ok<File> File::InDirectory(const char * path, const char * filename) {
		char fpath[FILENAME_MAX];
		strcpy(fpath, path);
		strcat(fpath, PathSplitter);
		strcat(fpath, filename);
		struct stat buffer;
		int exist = stat(fpath, &buffer);
		if(exist == 0) {
			File file;
			file.path = path;
			file.filename = filename;
			return file;
		}
		else return {}
	}
	#endif
}

#endif