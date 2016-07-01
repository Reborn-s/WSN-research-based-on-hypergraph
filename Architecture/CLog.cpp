
#include "CThread.h"
#include <iostream>
#include <fstream>
#include "CLog.h"

#ifdef WIN32
	#define sleep(x) Sleep((x)*1000) 
#endif

CLog::CLog()
{
	mpHead		 = NULL;
	mpTail		 = NULL;
	loopwork		 =true;
	mpLock		 = new CLock();
	ofstream pfileLog;
	pfileLog.open("..//clog.txt");    //clear clog.txt
	pfileLog.close();


}

bool CLog::addCLogWrite(CLogWrite *temp)
{
	mpLock->Lock();
	if(NULL == temp)
	{
		mpLock->Unlock();
		return false;
	}
	CLogWrite *tempCLog;
	tempCLog=new CLogWrite("");
	tempCLog=temp;
	tempCLog->setThreadID(getThreadID());
	if(mpHead == NULL){
		mpHead = tempCLog;
		mpTail = mpHead;
	}
	else{
		mpTail->setNext(tempCLog);
		mpTail = tempCLog; 
	}
	mpLock->Unlock();
	return true;
}

void CLogWrite::setNext(CLogWrite *temp)
{
	next=temp;

}

CLogWrite* CLogWrite::getNext()
{
	return next;

}

CLogWrite* CLog::getCLogWrite()
{
	mpLock->Lock();
	CLogWrite* getclogwrite;
	if(mpTail == NULL)
	{
		mpLock->Unlock();
		return NULL;
	}
	getclogwrite = mpHead;
	if(mpHead == mpTail)
		mpHead = mpTail = NULL;
	else
		mpHead = mpHead->getNext();
	mpLock->Unlock();
	return  getclogwrite;
}

CLogWrite::CLogWrite(const char str[])
{
	message=string(str);

}
void CLogWrite::setThreadID(std::string id)
{
	ThreadID=id;
}

CLogWrite::~CLogWrite()
{
	
}

void CLogWrite::writeLogFile()
{
	ofstream pfileLog;
	if(!pfileLog.is_open())
		pfileLog.open("..//clog.txt",ios::app);
	//cout<<message.c_str();
	string sy;
	sy=GenerateTimeStamp()+" \r "+ThreadID+" \r "+message;
	pfileLog.write(sy.c_str(),int(strlen(sy.c_str())));
	pfileLog.write("\n",1);
	pfileLog.close();

}

void* CLog::loop()
{	
	 CLogWrite *dowrite;
	 while(loopwork)
	 {
		 if(dowrite=getCLogWrite())
		 {
			dowrite->writeLogFile();
		 }
		 else
		{
			//cout<<"sleeping"<<endl;
			sleep(1);
		}
	 }	
	 return 0;

}

string CLogWrite::GenerateTimeStamp()
{
    time_t rawtime;
    struct tm timeinfo;// = NULL;
	
    char buffer[16];

    memset(buffer, 0, 16);
    time(&rawtime);
    //timeinfo = localtime(&rawtime);
    	#ifdef WIN32
		 localtime_s(&timeinfo, &rawtime);// may have wrong
		 strftime(buffer, 16, "%b %d %H:%M:%S", &timeinfo);
	#else
		struct tm * ptimeinfo;
		 ptimeinfo = localtime(&rawtime);
		 strftime(buffer, 16, "%b %d %H:%M:%S", ptimeinfo);
	#endif
    return string(buffer);
}

string CLog::getThreadID()
{
	std::ostringstream   ostr; 

	#ifdef WIN32
	ostr <<GetCurrentThreadId(); 
	string   str   =   ostr.str();
    return string(str);
	#else
	ostr <<pthread_self(); 
	string   str   =   ostr.str();
	 return string(str);
	#endif
}

void CLogNote(const char *note)
{
	extern CLog* dataLog;
	CLogWrite *p;
	p=new CLogWrite(note);
	dataLog->addCLogWrite(p);

}
