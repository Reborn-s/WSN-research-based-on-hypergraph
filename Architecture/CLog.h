#ifndef CLOG_H
#define CLOG_H

#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdarg>
#include <time.h>
#include <sstream>
#include "CLock.h"
#include "CThread.h"

using namespace std;

class CLogWrite
{
	protected:
		string message;
		string ThreadID;
		CLogWrite *next;
	public:
		CLogWrite(const char str[]);
		~CLogWrite();
		void writeLogFile();
		std::string GenerateTimeStamp();
		void setThreadID(std::string id);
		void setNext(CLogWrite *temp);
		CLogWrite *getNext();

};

class CLog
{		
	protected:
		 CLogWrite *mpHead;
		 CLogWrite *mpTail;
		 CLock*	mpLock;		 
		 bool loopwork;
	public:
		CLog();
		~CLog();
		bool addCLogWrite(CLogWrite *tempCLog);
		CLogWrite* getCLogWrite(void);
		std::string getThreadID();
		void *loop();
};
void CLogNote(const char note[]);
void *CLogThread(void* pParam);
#endif
