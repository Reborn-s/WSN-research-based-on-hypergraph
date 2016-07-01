/*
 * Project------ CArchitecture
 * Class Name--- Simplex.h
 * Author------- wxf891201@gmail.com
 * Date--------- 2013-4-8
 * Edition------ 1.0

 * Description-- This is a simplex structure, abstracting the attributes or methods or classes as a simplex object, 
 *		representing the relationship among these objects.

 * Change Log:
 *		Date----- 2013-4-9
 *		Staff---- wxf891201@gmail.com
 *		Edition-- 1.0
 *		Content-- Redefine the CSimplex class

 * Change Log:
 *		Date----- 2013-4-18
 *		Staff---- wxf891201@gmail.com
 *		Edition-- 1.0
 *		Content-- Add the struct stInvoke

*/
#ifndef SIMPLEX_H
#define SIMPLEX_H
#ifdef WIN32
#include "stdafx.h"
#else
#include <string.h>
#endif
#include <vector>
#include <map>
#include <fstream>
#include <string>
#include "define.h"
#include "stringnew.h"

using namespace std;
//class CArchitectureDoc;
class	CTestFile;

struct stAttribute
{
	string			attName;
	LPATTRIBUTE		attpNext;
	int				aflag;
};
/*
 * 成员方法的结构：返回类型 方法名 (参数列表) { 函数主体 };
*/
struct stMethod
{
	string		mthReturnType;
	string		mthName;
	//string		mthBrackt;
	string		mthParam;
	int			mthIndex;
	//string		mthBody;
	char*		mthBody;
	int			mthBodyLen;

	LPMETHOD	mthpNext;		// 当前类中函数链表的下一指针
	LPMETHOD	mthNextOnClass; // 一个类中的函数链
	LPMETHOD	mthNextOnDoc;	// 用于将多个类中的函数链表的头指针连接起来
	LPMETHOD	mthNextOnIvk;
	int			mflag;

	//stMethod ( int nIndex, string strName, string strBody );
	stMethod ( int nIndex, string strName , char* strBody, int bodyLen );
	void	outputMethodInvoke	( string fileName, LPMETHOD pMethod );
	//void	addInvokeMethod ( LPMETHOD pMth );
};

struct stClass
{
public:
	string		className;	
	//string	objName;		// 记录对象名，先只考虑一个类对应一个对象名的情况(后续考虑多个的情况需用string链表来放，见注释 )	
	/*string *	objName;
	string *	objNext;*/
	char*		classBody;
	int			clsBodyLen;

	vector <string> objName;
	
	LPCLASS		cpNext;
	int			m_cIndex;

	LPCLASS		cpNext_on_simplex;

	LPMETHOD	mthPhead;					// 类中函数的头结点

	LPTESTFILE	pCurrentFile;				// 当前类文件指针，便于对当前类中函数进行查找

	//stClass ( int index, string strClName, LPMETHOD pMethod, LPTESTFILE pCurrFile );
	stClass ( int index, string strClName, LPMETHOD pMethod, CTestFile* pCurrFile, char* clsBody, int bodyLen );

	void	outputClassToFile ( string fileName, LPCLASS pClass);

};

/*
 * 定义该数据结构是为了区别下述情况：
 * 如有三个类A,B,C, 类B和类C都作为类A的成员出现，而类B和C之间不确定有没有关系，需要根据函数调用进一步确定
 * 防止A,B,C出现在同一条边上，而是B,A及C,A两条边
*/
struct stRelation
{
	LPRELATION	pNextRelation;				// 以同一个类为中心的下一个有关联的关系类
	
	LPCLASS		pCls;						// 某一个关系上的类
	string		strName;					// 该关系上类名
	int			rClIndex;					// 该关系上类是哪个索引
	//LPSIMPLEX	pNextSimplex;				// 每一个关联关系链表上的下一个simplex

	stRelation ( string clName, int clIndex );
	~stRelation();
	//void addRelatedSimplexClass ( LPCLASS pstClass );
};

/*
 * 目的：表明函数间的调用关系
 * 即记录当前类中的每个成员函数调用了其他哪些类的哪些函数 
 * ( 记录的对象是method结构，每遇到一个函数间调用关系，就new一个该结构对象 )
 * 最终目的，是反过来查询，某个函数被其他哪些类的函数调用过
*/
struct stInvoke
{
	int			ivIndex;
	LPMETHOD	pMethod;
	LPINVOKE	pNextInvoke;	// 下一个调用关系
	LPMETHOD	pNextMethod;	// 调用关系上的下一个函数
	//LPCLASS		pClass;

	stInvoke ( int index, LPCLASS pCl, string mthName );
	~stInvoke();
};

//
class CSimplex
{
public:
	//LPATTRIBUTE		simplexAttribute;		// 关系类中的变量
	//LPMETHOD		simplexMethod;			// 关系类中的函数
	//LPCLASS			simplexClass;			// 类

	LPMETHOD		pMethodHead;
	LPMETHOD		pMethodTail;

	LPCLASS			pclassHead;
	LPCLASS			pclassTail;

	LPSIMPLEX		pNext;					// 关系链表的下一链表
	LPSIMPLEX		pNextByMth;
	int				m_iMthCnt;

	int				m_nIndex;				// 关系索引
	int				m_iCnt;					// 一个关系中类的计数

public:
	//Constructor ( each class corresponding a CSimplex )
	CSimplex ( int index,  LPCLASS pstClass );
	// Override constructor ( each method also corresponding a CSimplex )
	CSimplex ( LPMETHOD pMth );
	
	// 找到有调用的函数加到一条链表上
	void	addSimplexMethod ( LPMETHOD pMethod );
	// 找到有调用关系的函数加到一条链表上
	void	addSimplexClass ( LPCLASS pstClass );
	void	outputSimplexToFile ( string fileName, LPSIMPLEX pSimplex );
	~CSimplex ( );

};
#endif
