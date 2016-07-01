#include "CMgrJob.h"
#include <iostream>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CMgrJob :: CMgrJob()
{
	mpHead		 = NULL;
	mpTail		 = NULL;
	mpIOHead	 = NULL;
	mpIOTail	 = NULL;
	loopWorking	 = true;
	mpLock		 = new CLock();
	mpIOLock    = new CLock();
}

bool CMgrJob :: addJob(LPTESTCASE addjob)
{
	mpLock->Lock();
	if(NULL == addjob)
	{
		mpLock->Unlock();
		return false;
	}
	if(mpHead == NULL){
		mpHead = addjob;
		mpTail = mpHead;
	}
	else{
		mpTail->setNext(addjob);
		mpTail = addjob; 
	}
	mpLock->Unlock();
	return true;
}

bool CMgrJob :: addIOJob(LPTESTCASE addIOJob)
{
	mpIOLock->Lock();
	if(NULL == addIOJob)
	{
		mpIOLock->Unlock();
		return false;
	}
	if(mpIOHead == NULL){
		mpIOHead = addIOJob;
		mpIOTail = mpIOHead;
	}
	else{
		mpIOTail->setIONext(addIOJob);
		mpIOTail = addIOJob; 
	}
	mpIOLock->Unlock();
	return true;
}

LPTESTCASE CMgrJob :: getJob(){
	mpLock->Lock();
	LPTESTCASE getjob;
	if(mpTail == NULL)
	{
		mpLock->Unlock();
		return NULL;
	}
	getjob = mpHead;
	if(mpHead == mpTail)
		mpHead = mpTail = NULL;
	else
		mpHead = mpHead->getNext();
	mpLock->Unlock();
	return getjob;
}

LPTESTCASE CMgrJob :: getIOJob()
{
	mpIOLock->Lock();
	LPTESTCASE getOutJob;
	if(mpIOTail == NULL)
	{
		mpIOLock->Unlock();
		return NULL;
	}
	getOutJob = mpIOHead;
	if(mpIOHead == mpIOTail)
		mpIOHead = mpIOTail = NULL;
	else
		mpIOHead = mpIOHead->getIONext();
	mpIOLock->Unlock();
	return getOutJob;
}

void* CMgrJob :: dwLoopWork(){
	
	LPTESTCASE  dealJob;

	while(loopWorking){
		if(dealJob = getJob())
		{
			if(dealJob->getStage()==READ_END_STAGE)
			{
				dealJob->doParsing ( );
				dealJob->setStage(MODEL_END_STAGE);
				addIOJob(dealJob);
			}
			//addIOJob(dealJob);
			//sleep(1);
		}
		else
		{
			//cout<<"sleeping"<<endl;
			//sleep(1);
		}
		//cout<<"loopWorking"<<endl;
	}
	
	return 0;
}

void* CMgrJob :: dwLoopIOWork(){
	
	LPTESTCASE  IOJob;
	while(loopWorking){
		if(IOJob = getIOJob())
		{
			switch(IOJob->getStage())
			{
				case INIT_STAGE:			
					IOJob->doReading();
					IOJob->setStage(READ_END_STAGE);
					addJob(IOJob);
					break;
				case MODEL_END_STAGE:
					IOJob->doWriting();
					IOJob->setStage(WRITE_END_STAGE);
					delete IOJob;
					break;
				default:
					break;
			}
			//IOJob->doParsing(IOJob);
			//addIOJob(dealJob);
			//sleep(1);
		}
		else
		{
			//cout<<"sleeping"<<endl;
			//sleep(1);
		}
		//cout<<"loopWorking"<<endl;
	}
	
	return 0;
}

void CMgrJob :: setLoopWorking()
{
	 loopWorking = !loopWorking;
	 //cout<<loopWorking<<endl;
}

CMgrJob :: ~CMgrJob(){
	delete []mpLock;
}
