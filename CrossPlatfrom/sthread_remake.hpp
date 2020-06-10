#ifndef _STHREAD_H
#define _STHREAD_H

#ifdef WIN32
#include <windows.h>
#else
#include <pthread.h>
#include <signal.h>
#endif

namespace sthread {

	#ifdef WIN32
	typedef HANDLE ThreadType;
	#else
	typedef pthread_t ThreadType;
	#endif

	class Thread {
	public:
		ThreadType tHandle;
		Thread() {}
		virtual ~Thread();

		//Запустить поток без передачи управления
		void start();

		//Главная функция потока
		//Мб стоит через try catch ловить все исключения, иначе может навернутся просто на раз два
		virtual void action() = 0;

		//Просто join
		void join();

		//Отрубить потоку голову. Мб полетит и твоя голова, но это детали
		void kill();

	private:
		// Защита от случайного копирования
		Thread(const Thread&);
		void operator=(const Thread&);
	};

}

#endif