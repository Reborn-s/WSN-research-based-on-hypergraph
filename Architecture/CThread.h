#ifndef CTHREAD_H
#define CTHREAD_H
#ifndef WIN32
    #include <unistd.h>
	#include<pthread.h>
#else
	#include<windows.h>
#endif

class CThread
{
	#ifdef WIN32
		private:
			HANDLE thread;
		public:
			CThread(void* fun,void *lpParameter);
			HANDLE getThread();
			DWORD wait4ThisThread(int outTime);
			DWORD wait4ThisThread();
	#else
		private:
			pthread_t thread;
		public:
			CThread(void* (*fun)(void*),void *lpParameter);
			pthread_t getThread();
			void* wait4ThisThread();
	#endif
};
#endif


