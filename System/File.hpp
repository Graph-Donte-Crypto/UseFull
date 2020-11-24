#ifndef UF_SY_File_H
#define UF_SY_File_H

#if defined(WIN32)
#include <shlwapi.h>
#elif defined(UNIX)
#include <sys/stat.h>
#endif

#include <stdio.h>
#include <string.h>
#include <fstream>
#include <limits>

#include "../Utils/Ok.hpp"
#include "../String/BaseLib.hpp"
#include "../Templates/Span.hpp"
#include "../Templates/Array.hpp"

#include "Constants.hpp"

//UseFull System File module
//Version 1.0 alfa
//Make by Graph Don'te-Crypto

namespace ufsys {

	using namespace utils;

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
		static uft::Ok<size_t> GetSize(const char * filename);
		static uft::Span<char *> ReadAllLines(const char * filename);
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

	Ok<size_t> File::GetSize(const char * filename) {
		std::ifstream file;
		file.open(filename, std::ios::in | std::ios::binary);
		if (!file) {
			printf("unable to open [%s]\n", filename);
			return {};
		}
		file.ignore( std::numeric_limits<std::streamsize>::max() );
		size_t size = file.gcount();
		file.close();
		return size;
	}

	uft::Span<char *> File::ReadAllLines(const char * filename) {
		std::ifstream inf;
		inf.open(filename);

		if (!inf) {
			printf("unable to open [%s]\n", filename);
			return uft::Span<char *>();
		}

		uft::Array<char  > string_builder(64);
		uft::Array<char *> line_keeper(64);
		char last_readed_char;

		while (true) {
			last_readed_char = (char) inf.get();
			if (last_readed_char == LF || last_readed_char == CR) {
				string_builder.addCopy('\0');
				string_builder.optimizeMemory();
				char * string = (char *) string_builder.takeMemory();
				line_keeper.addCopy(string);
				string_builder.allocMemory(64);
			}
			else {
				string_builder.addCopy(last_readed_char);
			}
			if (inf.eof()) break;
		}

		inf.close();
		//string_builder.freeMemory();
		line_keeper.optimizeMemory();
		size_t length = line_keeper.length;
		char ** lines = (char **) line_keeper.takeMemory();
		return uft::Span(lines, length, true);
	}
}

#endif
