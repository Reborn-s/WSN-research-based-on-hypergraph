
#include "CThread.h"
#ifndef WIN32
	#include<pthread.h>
#else 
	#include<windows.h>
#endif



#ifdef WIN32
CThread :: CThread(void* fun,void *lpParameter)
{
	thread = CreateThread(0,0,(LPTHREAD_START_ROUTINE)fun,lpParameter,0,0);
}
HANDLE CThread :: getThread()
{
	return thread;
}
DWORD CThread :: wait4ThisThread(int outTime)
{
	return WaitForSingleObject(getThread(), outTime );
}
DWORD CThread :: wait4ThisThread()
{
	return WaitForSingleObject(getThread(),INFINITE );
}

#else
CThread :: CThread(void* (*fun)(void*),void *lpParameter)
{
	if(pthread_create(&thread,NULL,fun,lpParameter))
	{
		// cout<<"pthread_create error\n";
	}
}
pthread_t CThread :: getThread()
{
	return thread;
}

void* CThread :: wait4ThisThread()
{
	pthread_join (getThread(), NULL);
	return 0;
}
#endif
