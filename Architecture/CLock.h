#ifndef CLOCK_H
#define CLOCK_H
#include "define.h"


#include <stdio.h>

#ifdef WIN32
	#include <windows.h>
#else
    #include <unistd.h>
	#include<pthread.h>
#endif

class CLock
{
	public:
		CLock();
		void Lock();
		void Unlock();
		~CLock();

	private:
		#ifdef WIN32
			HANDLE hdLock;
			DWORD dwWaitResult;
		#else
			pthread_mutex_t mutexLock;
		#endif
};
#endif


