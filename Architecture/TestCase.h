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
#define END_NUM	        5				
#define INIT_STAGE				20
#define READ_END_STAGE		1	
#define MODEL_END_STAGE		2
#define WRITE_END_STAGE		3  

#define READ_FROM_HRG true

#include <string>
#include "define.h"
#include "CHypergraph.h"
#include "Multilevel.h"
#include "ReadH.h"
#include "ReadCpp.h"
#include "CInput.h"
#include <vector>
#include <map>
#include <queue>
//#include "GDI05Dlg.h"

#ifdef WIN32
#include "Shlwapi.h"
#include <direct.h>
#include <io.h>
#else 
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>
#include <sys/time.h>
#define sprintf_s sprintf
#endif
//#include "ArchitectureDoc.h"

using namespace std;
class calledclass
{
public:
	string name;
	int count;
	bool bIsmemberVar;   //判断是否是成员变量    
	calledclass *next;
	calledclass();
};



class CTestCase
{
public:
	int							stage;								//0表示初始阶段，1表示读完成，2表示处理完成，3表示写完成
	LPTESTCASE			m_pNext_test;
	LPTESTCASE			m_pIONext_test;
	CTestCase ();
	virtual ~CTestCase ( );

	void		setNext ( LPTESTCASE _nextcase );
	LPTESTCASE	getNext();

	void		setIONext ( LPTESTCASE _nextcase );
	LPTESTCASE	getIONext();

	virtual void		doReading ( );

	virtual void		doParsing ( );
	


	virtual void		doModeling ( );
	
	virtual void		doWriting ( );
	int			getStage();
	void		setStage(int s);
};

class CTestCaseCpp:public CTestCase
{
public:
	CReadCpp *head;
	string						strTestPath;
	string						strPara;
	string						testcasename;
	string						lastans;
	CHypergraph					*pHyper;
	CInput						*dataIn;
	CMygraph					*pMyGraph;
	int							cutnum;
	int							allTime;
	vector<int>					PartsResult;
	vector<string>				NodesResult;
	vector<double>				ModuList;
	vector<double>				CohenList;
	#ifdef WIN32
	DWORD		runtime;
	#else
	timeval		runtime;
	#endif
	string  writeTime;
	map<string,calledclass*> calledrelation;
	map<string,string> mapForTypeDef;
	vector < string >	arycpp_fileName;
	vector < string >	aryh_fileName;
	map <string, CReadH*> hmap;
	map<string,string> fathermap;
	map<int,int> mapForLastPart;
	vector<string> VectorV;
	map<string,int> classtonum;
public:
	CTestCaseCpp ( int index, string strNameTest);
	~CTestCaseCpp ( );


	void		doReading ( );

	void		doParsing ( );
	


	void		doModeling ( );
	
	void		doWriting ( );

	string    findtype(string content,string valuename,int endpos);
	void      findsymbol(string symbol,string methodbody,string classname,CReadCpp *p);
	void		InitCalledParameter(CMethodBody *p);
	void      readEachfile();
	void		createHypergraph();
	CHypergraph*     createCoarseningHgraph(int nvtxs，,vector<WSNnode>& nodes);
	void removeIsolatePoint();
	void     coarseningToCluster(int parts,map<int,int> moduleNodeNumber);
	void     selectToCluster(int parts);
	void		createHypergraphFromHrg(string filename);
	void		doHmetis();
	void		createSubGraph(CHypergraph *G,CHypergraph *subG1,CHypergraph *subG2);
	void		dealwithTypeDef(string content);
	void		dfsFolder(string path);
	vector<string> split(string str,string pattern);
	int        dfsForWriteParts(CHypergraph *G,int parts);
	void		mergeParts(int partnum);
	void    callMatlab();
	vector<double>		ComputeMoDuList(vector<int> part_res);
	double		ComputeMoDuBtwTwoParts(vector<int> part_res,CMygraph *pInitGraph,int PartOne,int PartTwo);
	double		ComputeTotalCompling(vector<int> part_res,CMygraph *pInitGraph,int PartOne);
};


#endif
