#include "sthread.h"

namespace sthread {
	static void ThreadCallback(Thread * who) {
		#ifndef WIN32
		// Далаем поток "убиваемым" через pthread_cancel.
		int old_thread_type;
		pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &old_thread_type);
		#endif
		who->action(); 
	}

	#ifdef WIN32
	//Для винды
	Thread::~Thread() {
		CloseHandle(tHandle);
	}

	void Thread::start() {
		tHandle = CreateThread(0, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(ThreadCallback), this, 0, 0);
	}

	void Thread::join() {
		WaitForSingleObject(tHandle,  INFINITE);
	}

	void Thread::kill() {
		TerminateThread(tHandle, 0);
	}

	#else
	//Для остального мира
	Thread::~Thread() {
	}

	extern "C" typedef void *(* pthread_callback)(void *);

	void Thread::start() {
		pthread_create(&tHandle, 0, reinterpret_cast<pthread_callback>(ThreadCallback), this);
	}

	void Thread::join() {
		pthread_join(tHandle, 0);
	}

	void Thread::kill() {
		pthread_cancel(tHandle);
	}

	#endif

}