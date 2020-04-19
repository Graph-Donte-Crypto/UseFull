#ifndef UF_U_Trace_H
#define UF_U_Trace_H

#include <stdio.h>
#include <stdlib.h>
#include <exception>
#include <csignal>

#include "Prefix.h"

namespace utils {

	//Узнать про backtrace

	static 
	struct TraceClass {
		const char * names[2048];
		const char * files[2048];
		      int    lines[2048];
		size_t index = 0;
		bool active = true;

		TraceClass() {}
		~TraceClass() {
			if (active) {
				printTrace();
			}
		}

		void push(const char * name, const char * file, const int   line) {
			names[index] = name;
			files[index] = file;
			lines[index] = line;
			index++;
		}

		void pop() {
			index--;
		}

		void printTrace() {
			for (size_t i = 0; i < index; i++)
				printf("%s: %i: %s\n", files[i], lines[i], names[i]);
		}

		void exit(int error = 0) {
			active = false;
			printTrace();
			::exit(error);
		}
	}
	Trace;

	void TracerSIGSEGV(int a) {
		prefix_unused(a);
		printf("Segmentation Fault:\n");
		Trace.exit();
	}

	void setTracer() {
		std::signal(SIGSEGV, TracerSIGSEGV);
	}
	

	#define trace(func) Trace.push(prefix_stringify(func), __FILE__, __LINE__); func Trace.pop();

}

//#define LOG_TRACE printf("Entering %s() (%s:%d)\n", __FUNCTION__, __FILE__, __LINE__);

#endif