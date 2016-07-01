// CArchitectureDoc.cpp : implementation of the CArchitectureDoc class
//
/*
 * Project------ CArchitecture
 * Class Name--- CArchitectureDoc.cpp
 * Author------- wxf891201@gmail.com
 * Date--------- 2013-3-30
 * Edition------ 1.0

 * Description-- MFC的文档,一个Doc对应一个打开的.h或.cpp文件。
 *		对连续打开的Doc进行解析，解析出其中包含的类，并存于类结构中，便于后续生成simplex对象

 * Change Log:
 *		Date----- 2013-4-13
 *		Staff---- 
 *		Edition-- 
 *		Content-- 对.h文件按行分析，查找符合函数签名的部分，并提取出函数名，即查找成员函数的过程

 * Change Log:
 *		Date----- 2013-4-17
 *		Staff---- 
 *		Edition-- 
 *		Content-- 重新定义了查找函数的功能。对.h和.cpp文件按行分析，查找符合函数定义的部分，提取函数名

 * Change Log:
 *		Date----- 2013-4-23
 *		Staff---- 
 *		Edition-- 
 *		Content-- 函数查找处，使用GlobalAlloc分配方式，手动管理函数体的存储
*/

#include "stdafx.h"
#include "Architecture.h"

#include "ArchitectureDoc.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CArchitectureDoc

IMPLEMENT_DYNCREATE(CArchitectureDoc, CDocument)

BEGIN_MESSAGE_MAP(CArchitectureDoc, CDocument)
END_MESSAGE_MAP()


// CArchitectureDoc construction/destruction

CArchitectureDoc::CArchitectureDoc()
{
	/*strFile			= "";
	m_strText		= "";
	m_cIndex		= 1;
	m_pClass_doc	= NULL;

	m_mIndex		= 1;
	m_mpHead		= NULL;*/
}

CArchitectureDoc::~CArchitectureDoc()
{
	//此处要处理stClass的释放
	/*LPCLASS	pClass;
	pClass		= m_pClassHead;
	while ( pClass )
	{
		m_pClassHead	= pClass->cpNext;
		delete			pClass;
		pClass			= m_pClassHead;
	}*/
}

BOOL CArchitectureDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}

// CArchitectureDoc serialization

void CArchitectureDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CArchitectureDoc diagnostics

#ifdef _DEBUG
void CArchitectureDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CArchitectureDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CArchitectureDoc commands
/*
 * 获取多个连续打开的文件名，并对该文件按行读取操作，找出其中的类
*/
//void CArchitectureDoc::findClass ( string fileName )
//{
//	CStdioFile	file;
//	string		strLine;
//
//	strFile		= fileName;
//	file.Open ( strFile, CFile::modeRead );
//	strLine		= "";
//
//	while ( readString (&file, &strLine) )
//	{
//		// 对每行字符串进行解析，找出含有class关键字的地方，并记录跟在class后边的类名
//		// 可能要改成switch，class int等关键字作为分支，需要细化
//		int		strPosition;
//		string strNameClass;
//		string strKeyword;
//		int		keyword_size;
//		strKeyword		= _T("class ") ;
//		keyword_size	= strKeyword.GetLength();
//		// 查找所有类
//		strPosition		= strLine.Find( strKeyword );
//		if ( -1 != strPosition)
//		{
//			//提取关键字class后面的类名
//			strNameClass	= strLine.Mid( strPosition+ keyword_size );
//			
//			m_pClass_doc		= new stClass ( m_cIndex, strNameClass, NULL, this );
//			// 可能一个.h中会有多个类的声明，所以这里需要将同一个.h中的类连起来，类似查找函数的做法
//
//#ifdef	_DEBUG_OUTPUT
//			//outputClassToFile ("..//output//classes.txt", m_pClass_doc );
//#endif
//
//#ifdef	_DEBUG_AFXMESSAGE
//			AfxMessageBox (strNameClass);
//#endif
//		}
//		
//	}
//	file.Close();
//	findMethod (strFile);
//}

/*
 * 获取多个连续打开的文件名，并对该文件按行读取操作，找出其中的函数
 * 找函数的思路：先找"("")"，包含括号对的语句可能有以下情况：
 * 1.控制语句 ( if, while, for, switch )
 * 2.各种情况的函数调用
 * 3.真正所要找的函数定义
 * 通过排除该行代码不含1中所提的关键字，可以排除情况1；通过继续往后寻找花括号对{ }，可排除情况2
 * 综上，逐行代码分析 if ( 该行含有括号对 )，无则继续往下行分析，有则查该行是否含有控制语句关键字；
 * if (含关键字)，则继续往下行分析，找到控制语句结尾的花括号对；不含关键字，则继续往下行分析花括号对，
 * 直到花括号配对成功，则为函数定义
*/

//LPMETHOD CArchitectureDoc::findMethod( string fileName )
//{
//	CStdioFile	pFile;
//	string		strMthLine;						// 按行读取的内容
//	string		strMthText;						// 存某一段函数定义主体
//	string		strMethod;
//	LPMETHOD	pFind;
//	int			iLBracket;
//	int			iRBracket;
//	int			nBrace;							// 记录花括号对
//
//	HGLOBAL		hMem;
//	char*		pStr;
//
//	pFile.Open ( fileName, CFile::modeRead );
//
//	pFind		= NULL;
//	pStr		= NULL;
//	strMthLine	= "";
//	strMthText	= "";
//	nBrace		= 0;
//
//	//while ( pFile.ReadString( strMthLine ) )
//	while ( readString ( &pFile, &strMthLine ) )
//	{
//		//omitComment ( &strMthLine );
//		// 滤去空行
//		if ( strMthLine == "" )
//			continue;
//
//		iLBracket	= strMthLine.Find( _T("(") );
//		iRBracket	= strMthLine.Find( _T(")") );
//		// 首先查找括号对
//		if ( -1 != iLBracket && -1 != iRBracket )
//		{
//			// 查找到控制语句关键字
//			if ( findToken ( strMthLine ) )
//			{
//				continue;
//			}
//			// 可能是函数调用
//			else if ( -1 != strMthLine.Find(_T(";")) )
//			{
//				continue;
//			}
//			//函数定义部分
//			else
//			{
//				strMthLine.TrimLeft (" ");
//				strMthLine.TrimRight (" ");
//				strMethod	= strMthLine.Left ( iLBracket );
//				//strMthText	+= strMthLine;
//#ifdef _DEBUG_APPEND 
//
//				strMthText.Append ( strMthLine );
//				strMthText.Append ( "\n" );
//#endif			
//				// 手动管理存储函数体的内存块
//				int			iShift;
//				int			dwBytes;
//
//				iShift		= 0;
//				pStr		= NULL;
//				dwBytes		= strMthLine.GetLength ( ) + 1;
//				hMem		= ::GlobalAlloc ( GHND, dwBytes );				
//				pStr		= (char*)::GlobalLock ( hMem );
//				::memcpy ( pStr, strMthLine.GetBuffer(), dwBytes );
//				//pStr [dwBytes - 1] = '\n';
//				//AfxMessageBox(pStr);
//				iShift		= strMthLine.GetLength() + 1;
//				//pStr		= pStr + iShift;
//				
//				::GlobalUnlock ( hMem );
//
//				//::GlobalFree ( hMem );
//				//strMthText	+= "\n"; 
//				// 继续往后读
//				//while ( pFile.ReadString (strMthLine) )
//				while ( readString ( &pFile, &strMthLine ) )
//				{
//					//omitComment ( &strMthLine );
//					strMthLine.TrimLeft (" ");
//					strMthLine.TrimRight (" ");
//					int			dwBytes_new;
//					int			sLen;
//					dwBytes_new	= strMthLine.GetLength ( ) + iShift + 1;
//					sLen		= strMthLine.GetLength ( ) ;
//					hMem		= ::GlobalReAlloc ( hMem, dwBytes_new , GHND );
//					pStr		= (char*) ::GlobalLock ( hMem ); 
//					pStr		= pStr + iShift;
//					::memcpy ( pStr, strMthLine.GetBuffer(), sLen );
//					//pStr [sLen]	= '\n';
//					iShift		+= strMthLine.GetLength ( ) + 1;
//					//pStr		= pStr + iShift;
//					//AfxMessageBox(pStr);
//					
//					::GlobalUnlock ( hMem );
//#ifdef _DEBUG_APPEND 
//					strMthText.Append(strMthLine);
//					strMthText.Append("\n");
//#endif
//					//strMthText	+= strMthLine;
//					//strMthText	+= "\n";
//					if ( -1 != strMthLine.Find(_T("{")) )
//					{
//						nBrace ++;
//						continue;
//					}
//					if ( -1 != strMthLine.Find(_T("}")) )
//					{
//						nBrace --;
//						continue;
//					}
//					if ( 0 == nBrace )
//					{
//						/*int		iColon, iSpace;
//						iColon	= strMethod.Find (_T("::"));
//						iSpace	= strMethod.Find (_T(" "));
//						if ( -1 != iColon )
//						{
//							string mthClass	= strMethod.Mid ( iSpace, iColon );
//							AfxMessageBox (mthClass);
//						}*/
//						//strMethod	= strMthText.Left ( iLBracket );
//						//
//						hMem			= ::GlobalReAlloc ( hMem, iShift , GHND );
//						char* pStrBody	= (char*)::GlobalLock ( hMem );
//						//pStrBody [iShift-1]	= '\0';
//
//						//AfxMessageBox (pStrBody);
//#ifdef _DEBUG_APPEND 
//						pFind			= new stMethod ( m_mIndex, strMethod, strMthText );
//#endif
//						pFind			= new stMethod ( m_mIndex, strMethod, pStrBody, iShift );
//						//m_pSimMethod	= new CSimplex ( pFind );
//						pFind->mthpNext	= m_mpHead;
//						m_mpHead		= pFind;
//						/*m_pSimMethod->pNextByMth	= m_pSimMthHead;
//						m_pSimMthHead				= m_pSimMethod;*/
//
//						pStr	= NULL;
//						//AfxMessageBox ( strMthText );
//						//strMthText	= "";
//						//AfxMessageBox ( strMethod );
//						break;		// 跳出当前读取字符的循环
//					}	
//				} // end inside while
//				continue;			// 从当前位置继续往下读
//			} // end else
//		} // end if
//		// 如果第一次遇到的不是包含有括号的行，就继续往下读
//		else
//			continue;
//	} // end outside while
//	pFile.Close ( );
//	return pFind;
//
//}

// 该函数的作用是按行遍历文件，分析每一行是否为注释，为注释返回false，非注释返回true，即为有效行
// 要考虑到的一种特殊情况，函数内参数含有注释 void method ( /* int param */ )的情况
//bool CArchitectureDoc::readString ( CStdioFile* pFile, string* strLine )
//{
//	int		flagCommom_1;
//	int		flagCommom_2; 
//	int		flagCommom_3;
//	int		flag_pre;
//
//	while ( pFile->ReadString ( *strLine ) )
//	{
//		flagCommom_1	= strLine->Find(_T("//"));		
//		flagCommom_2	= strLine->Find(_T("/*"));		
//		flag_pre		= strLine->Find(_T("#"));
//		// 判断行注释
//		if ( -1 != flagCommom_1 )
//		{
//			continue;
//		}
//		// 判断段注释, 其中包括函数参数内含注释的特殊情况
//		if ( -1 != flagCommom_2 )
//		{
//			flagCommom_3	= strLine->Find(_T("*/"));
//			if ( -1 != flagCommom_3 )
//			{
//				strLine->Delete ( flagCommom_2, flagCommom_3 - flagCommom_2 + 2 );
//				return true;
//			}
//			else
//			{
//				while ( pFile->ReadString ( *strLine ) )
//				{
//					flagCommom_3	= strLine->Find(_T("*/"));
//					if ( -1 != flagCommom_3 )
//					{
//						break;
//					}
//					continue;
//				}
//			
//			}
//			continue;
//		}
//		// 判断是否为预处理行，包括#include
//		if ( -1 != flag_pre )
//		{
//			continue;
//		}
//		// 若该行内容为空也继续读下行
//		/*if ( strLine->IsEmpty() )
//		{
//			continue;
//		}*/
//		return true;
//	}
//	// 均不符合上述情况时，默认情况下返回false
//	return false;
//}
// 此函数的作用是：在给定的字符串中查找某些指定的子串，便于扩展。可改为内联函数，后续考虑
//bool CArchitectureDoc::findToken ( string str )
//{
//	if ( -1 != str.Find ( _T("if") ) )
//		return true;
//	if ( -1 != str.Find ( _T("while") ) )
//		return true;
//	if ( -1 != str.Find ( _T("for") ) )
//		return true;
//	if ( -1 != str.Find ( _T("switch") ) )
//		return true;
//
//	return false;
//}

//void CArchitectureDoc::outputClassToFile ( string fileName, LPCLASS pClass )
//{
//	//char	cpClass [128];
//	char	cpstrClass [128];
//
//	ofstream	pfile;
//	pfile.open ( fileName.c_str(), ios::app );
//
//	if ( pClass )
//	{
//		/*sprintf_s ( cpClass, 128, "%d ", pClass->m_cIndex );
//		pfile.write ( cpClass, strlen (cpClass) );*/
//		//AfxMessageBox(_T("write")+pClass->className);
//		sprintf_s ( cpstrClass, 128, "%s ", pClass->className );
//		pfile.write ( cpstrClass, (int) strlen (cpstrClass) );
//		pfile.write ( "\n", 1 );
//	}
//	pfile.close();
//}