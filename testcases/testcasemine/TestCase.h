/*
 * Project------ CArchitecture
 * Class Name--- TestCase.h
 * Author------- wxf891201@gmail.com
 * Date--------- 2013-5-8
 * Edition------ 1.0

 * Description-- 每一个testcase对应一个TestCase类，在该类中可以进行多种操作，如parser，超图建模。
 * 原本在Doc和Mainfrm中实现，现在全部封装在TestCase类中。
*/
#ifndef	TESTCASE_H
#define	TESTCASE_H

#define INIT_STAGE			0
#define READ_END_STAGE		1	
#define MODEL_END_STAGE		2
#define WRITE_END_STAGE		3
#include <string>
#include "define.h"
#ifdef WIN32
#include "stdafx.h"
#include "Shlwapi.h"
#include "ReadH.h"
#include "ReadCpp.H"
#include <vector>
#include <map>
#else 
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <sys/stat.h>
#include <dirent.h>
#endif
//#include "ArchitectureDoc.h"

using namespace std;
class CTestCase
{
public:
	CReadCpp *head;
	LPTESTCASE			m_pNext_test;
	LPTESTCASE			m_pIONext_test;
	int							stage;								//0表示初始阶段，1表示读完成，2表示处理完成，3表示写完成
	string						strTestPath;
	string						lastans;
	vector < string >	arycpp_fileName;
	vector < string >	aryh_fileName;
	map <string, CReadH*> hmap;
public:
	CTestCase ( int index, string strNameTest );
	~CTestCase ( );

	void		setNext ( LPTESTCASE _nextcase );
	LPTESTCASE	getNext();

	void		setIONext ( LPTESTCASE _nextcase );
	LPTESTCASE	getIONext();

	void		doReading ( );

	void		doParsing ( );
	


	void		doModeling ( );
	
	void		doWriting ( );
	int			getStage();
	void		setStage(int s);
	string    findtype(string content,string valuename,int endpos);
	void      findsymbol(string symbol,string methodbody,string classname,CReadCpp *p);
	//void		HypergraphVertex();
	
};

#endif
