#ifndef UF_S_Part_H
#define UF_S_Part_H

#include "BaseLib.hpp"
#include "S.hpp"
//UseFull String Part module 
//Version 1.0 alfa
//Make by Graph Don'te-Crypto

namespace str {
	//TODO: модуль Part очень сложен для понимания
	//Для каждого метода нужно сделать пояснение и привести примеры использовани
	namespace Part {

		namespace Next {
			//Возвращает указатель на первый символ после splitter, если таковых нет - возвращает nullptr
 			char * Skip(char * in, char splitter) {
				while (*in != splitter) {
					if (*in == 0) return nullptr;
					in++;
				}
				return *in ? in + 1 : in;
			}

			char * SkipLevelSame(char * in, char splitter, char same) {
				if (*in == 0) return nullptr;
				bool f = true;
				while (true) {
					if (*in == same) f = !f;
					if ( (*in == splitter && f) || *in == 0) break;
					in++;
				}
				return *in ? in + 1 : in;
			}
			//Возвращает указатель на первый символ после splitter, если in[0] == 0 возвращает nullptr
			//Строку между in и первым символом splitter пишет в out
			char * Get(char * in, char * out, char splitter = ' ') {
				if (*in == 0) return nullptr;
				while (true) {
					*out = *in;
					if (*out == splitter || *out == 0) break;
					out++;
					in++;
				}
				*out = 0;
				return *in ? in + 1 : in;
			}
			#ifdef UF_U_Context_H
			/*

			Функция получается следующую подстроку из in, разделённую splitter в out,
			возвращает смещеную in и bool значение - можно ли интерироваться дальше

			//main argmunet - char * - Новая позиция строка
			//0    argmunet - bool   - Можно ли применять функцию дальше
	
			CodeExample:
			while (true) {
				contextCopyCharPtr = Part::Next::GetContext(charPtr, outStringS, delimiter);
				if (contextCopyCharPtr.getValue<bool>(0) == false) break;
				charPtr = (char *)contextCopyCharPtr;
			}
			На каждой итерации кладёт кусочек строки charPtr, разделённый delimiter в outStringS,
			до тех пор, пока не кончаться все кусочки
			*/
			utils::ContextCopy<char *> GetContext(char * in, S & out, char splitter = ' ') {
				utils::ContextCopy<char *> ret;
				if (in[0] == 0) {
					ret.record = nullptr;
					ret.addValue(false);
					return ret;
				}
				else ret.addValue(true);
				char find[] = {splitter, 0};
				char * pos = PosMin(in, find);
				size_t nlen = 0;
				if (pos == nullptr){nlen = strlen(in); ret.getRef<bool>(0) = false;}
				else                nlen = pos - in;
				out.set(in, nlen);
				ret.record = *in ? in + nlen + 1 : in;
				return ret;
			}
			#endif
			//Возвращает указатель на первый символ после splitter, если in[0] == 0 возвращает nullptr
			//Строку между in и первым символом splitter пишет в out, предварительно выделев под него память
			char * GetNew(char * in, char *& out, char splitter = ' ') {
				if (*in == 0) return nullptr;
				char * inptr = in;
				size_t count = 0;
				while (true) {
					count++;
					if (*in == splitter || *in == 0) break;
					in++;
				}

				out = new char[count];
				strncpy(out, inptr, count-1);
				out[count-1] = 0;
				return *in ? in+1 : in;
			}
			/*
			Возвращает указатель на первый символ после splitter, если in[0] == 0 возвращает nullptr
			Строку между in и первым символом splitter пишет в out, предварительно выделев под него память
			при нахождение символа open lvl увеличивается на 1, при нахождение close - уменьшается на 1
			splitter учитывается только если lvl == 0
			Примеры кода:
			GetLevelDual("Hello (my dear(world)) afterwhat", str, ' ', '(', ')', 0) -> str == Hello;
			GetLevelDual("(my dear(world)) afterwhat", str, ' ', '(', ')', 0) -> str = (my dear(world))
			GetLevelDual("afterwhat", str, ' ', '(', ')', 0) -> str = afterwhat
			*/
			
			char * GetLevelDual(char * in, char * out, char splitter, char open, char close, int lvl = 0) {
				if (*in == 0) return nullptr;
				while (true) {
					*out = *in;
					if (*out == open) lvl++;
					else if (*out == close) lvl--;
					if ( (*out == splitter && lvl == 0) || *out == 0) break;
					out++;
					in++;
				}
				*out = 0;
				return *in ? in + 1 : in;
			}
			char * GetLevelSame(char * in, char * out, char splitter, char same) {
				if (*in == 0) return nullptr;
				bool f = true;
				while (true) {
					*out = *in;
					if (*out == same) f = !f;
					if ( (*out == splitter && f) || *out == 0) break;
					out++;
					in++;
				}
				*out = 0;
				return *in ? in + 1 : in;
			}
		}
		//TODO: remake PREV: make return like NEXT & make ..Level && ..LevelSame functions
		namespace Prev {

			char * Get(char * str_start, char * in, char target) {
				while (true) {
					if (in == str_start) return nullptr;
					if (*in == target) break;
					in--;
				}
				return in-1;
			}
			char * Get(char * str_start, char * in, char * out, char splitter = ' ') {
				char * buf_in = in;
				char * ret = buf_in;
				while (true) {
					if (*buf_in == splitter) break;
					if (buf_in == str_start) return nullptr;
					buf_in--;
				}
				ret = buf_in - 1;
				buf_in++;
				while (true) {
					*out = *buf_in;
					out++;
					if (buf_in == in) break;
					buf_in++;
				}
				*out = 0;
				return ret;
			}
		}
	}
}
#endif