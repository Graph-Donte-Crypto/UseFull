#ifndef UF_S_File_H
#define UF_S_File_H

#include "BaseLib.hpp"

#include "../Utils/Context.hpp"

#include "Replace.hpp"
#include "Part.hpp"
#include "S.hpp"



//UseFull String File module 
//Version 1.2 alfa
//Make by Graph Don'te-Crypto

using namespace utils;

namespace str {

	namespace File {
		//main argmunet - char * - string begin
		//0    argmunet - size_t - string length
		ContextCopy<char *> Read(const char * filename) {
			FILE * f = fopen(filename, "rb");
			if (f) {
				fseek(f, 0, SEEK_END);
				size_t size = ftello64(f) + 1;
				fseek(f, 0, SEEK_SET);
				char * file = new char[size];
				ContextCopy<char *> ret(file);
				ret.addRef(size);
				fread(file, 1, size - 1, f);
				file[size - 1] = 0;
				fclose(f);
				return ret;
			}
			else {
				printf("File <%s> open error\n", filename);
				exit(1);
			}
		}

		S ReadS(const char * filename) {
			FILE * f = fopen(filename, "rb");
			if (f) {
				fseek(f, 0, SEEK_END);
				size_t size = ftello64(f) + 1;
				S file(size);
				fseek(f, 0, SEEK_SET);
				fread(file.ptr, 1, size - 1, f);
				file[size - 1] = 0;
				fclose(f);
				return file;
			}
			else {
				printf("File <%s> open error\n", filename);
				exit(1);
			}
		}

		char * ReadDelimitedFile(const char * filename, const char delimiter = (char)10) {
			char * file = File::Read(filename);

			char d1[] = {delimiter, 0};
			char d2[] = {delimiter, delimiter, 0};

			if (delimiter == 10) {//if delimiter == LF -> For CrossPlatform support replace CR -> LF
				char d3[] = {13, 0};
				Replace::Standart(file, d3, d1);
			}

			Replace::Repeat(file, d2, d1);
			
			if (file[0] == delimiter) Shift(file, -1);
			char & last = Last(file);
			if (last == delimiter) last = 0;
			return file;
		}

		//main argmunet - char ** - string array begin
		//0    argmunet - size_t  - string array length
		ContextCopy<char **> ReadStringsContext(const char * filename, const char delimiter = (char)10) {
			char * file = ReadDelimitedFile(filename, delimiter);
			size_t str_count = CountChar(file, delimiter) + 1;

			char ** arr = new char * [str_count];
			ContextCopy<char **> ret(arr);
			ret.addRef(str_count);
			char * ptrin = file;
			size_t count = 0;
			while ((ptrin = Part::Next::GetNew(ptrin, arr[count], delimiter)) != nullptr) {
				count++;
			}
			delete[] file;
			return ret;
		}
		//main argmunet - S *     - S array begin
		//0    argmunet - size_t  - S array length
		ContextCopy<S *> ReadStringsContextS(const char * filename, const char delimiter = (char)10) {
			char * file = ReadDelimitedFile(filename, delimiter);
			size_t str_count = CountChar(file, delimiter) + 1;

			S * arr = new S[str_count];

			ContextCopy<S *> ret(arr);
			ret.addRef(str_count);

			char * ptrin = file;
			ContextCopy<char *> spng;
			size_t count = 0;
			while (true) {
				spng = Part::Next::GetContext(ptrin, arr[count], delimiter);
				if (spng.getValue<bool>(0) == false) break;
				ptrin = (char *)spng;
				count++;
			}

			delete[] file;
			return ret;
		}

        char ** ReadStrings(const char * filename, const char delimiter = (char)10) {
			char * file = ReadDelimitedFile(filename, delimiter);
			size_t str_count = CountChar(file, delimiter) + 1;

            char ** arr = new char * [str_count];
            char * ptrin = file;
            size_t count = 0;
            while ((ptrin = str::Part::Next::GetNew(ptrin, arr[count], delimiter)) != nullptr) {
                if (arr[count][0]) count++;
                else {delete[] arr[count];}
            }
            arr[count] = new char[1];
            arr[count][0] = 0;
            delete[] file;
            return arr;
        }

        void Rewrite(const char * filename, const char * file) {
            FILE * f = fopen(filename, "w");
            if (f) {
                fwrite(file, 1, strlen(file), f);
                fclose(f);
            }
            else {
                printf("File %s rewrite error\n", filename);
                exit(1);
            }
        }
        void Append(const char * filename, const char * file) {
            FILE * f = fopen(filename, "a");
            if (f) {
                fwrite(file, 1, strlen(file), f);
                fclose(f);
            }
            else {
                printf("File %s append error\n", filename);
                exit(1);
            }
        }
        bool Exist(const char * filename) {
        	FILE * f = fopen(filename, "r");
        	if (f) {
     			fclose(f);
        		return true;
        	}
        	else return false;
        }
	}

	void FileTest(const char * filename) {
		printf("start reading\n");
		char * s1 = str::File::Read(filename);
		printf("s1 readed\n");
		str::S s2 = str::File::ReadS(filename);
		printf("s2 readed\n");
		ContextCopy<char **> s3 = str::File::ReadStringsContext(filename);
		printf("s3 readed\n");
		ContextCopy<str::S *> s4 = str::File::ReadStringsContextS(filename);
		printf("s4 readed\n");
		char ** s5 = str::File::ReadStrings(filename);
		printf("s5 readed\n");

		printf("s1 print\n");
		printf("%s\n", s1);
		printf("------------------\n");
		printf("s2 print\n");
		printf("%s\n", s2.ptr);
		printf("------------------\n");
		printf("s3 print\n");
		for (size_t i = 0; i < s3.getValue<size_t>(0); i++) printf("%s\n", ((char **)s3)[i]);
		printf("------------------\n");
		printf("s4 print\n");
		for (size_t i = 0; i < s4.getValue<size_t>(0); i++) printf("%s\n", ((str::S *)s4)[i].ptr);
		printf("------------------\n");
		printf("s5 print\n");
		for (size_t i = 0; s5[i][0] != 0; i++) printf("%s\n", s5[i]);
	}

}

#endif 