// ArchitectureDoc.h : interface of the CArchitectureDoc class
//

#pragma once

#ifndef ARCHITECTURE_H
#define	ARCHITECTURE_H

#include "Simplex.h"

class CArchitectureDoc : public CDocument
{
//protected: // create from serialization only
public:
	CArchitectureDoc();
	DECLARE_DYNCREATE(CArchitectureDoc)

// Attributes
//public:
//	CArchitectureDoc	*m_pNextDoc;			// Doc的下一指针，用于将多个Doc文档连起来
//
//	string				strFile;				// 所要打开的文件名
//
//	string				m_strText;
//	int					m_cIndex;
//	LPCLASS				m_pClass_doc;			// 当前Doc中的类
//
//	int					m_mIndex;				// 函数索引
//	LPMETHOD			m_mpHead;				// 每一个文档的函数头
//
//// Operations
//public:
//	void		findClass ( string fileName );
//	//对每一行文本进行分析，满足函数的定义部分，则归为成员函数，返回查找到的函数
//	LPMETHOD	findMethod ( string fileName );	
//	bool		readString ( CStdioFile* pFile, string* strLine );
//	bool		findToken ( string str );
//	void		outputClassToFile ( string fileName, LPCLASS pClass);
	
// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CArchitectureDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#endif


