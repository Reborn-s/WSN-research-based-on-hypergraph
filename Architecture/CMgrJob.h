#ifndef CMGRJOB_H
#define CMGRJOB_H


#ifdef WIN32
	#define sleep(x) Sleep((x)*1000) 
#endif

#include "CLock.h"
#include "CThread.h"
#include "TestCase.h"
#include "define.h"

class CMgrJob
{
	public:
		CMgrJob();
		bool addJob(LPTESTCASE addjob);
		LPTESTCASE getJob();
		void* dwLoopWork();
		void* dwLoopIOWork();
		void setLoopWorking();

		LPTESTCASE getIOJob();
		bool addIOJob(LPTESTCASE addIOJob);

		~CMgrJob();
		
	private:
		LPTESTCASE	mpIOHead;
		LPTESTCASE	mpIOTail;

		LPTESTCASE	mpHead;
		LPTESTCASE	mpTail;
		bool	loopWorking;
		CLock*	mpLock;
		CLock*	mpIOLock;
};
void* loopJob(void* pParam);
#endif


