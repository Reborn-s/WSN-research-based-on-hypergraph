// Architecture.h : main header file for the CArchitecture application
//
#pragma once

#ifndef __AFXWIN_H__
//	#error "include 'stdafx.h' before including this file for PCH"
#endif
#ifdef WIN32
    #include "resource.h"
#endif
// main symbols

#include "TestCase.h"
#include "CMgrJob.h"

// CArchitectureApp:
// See CArchitecture.cpp for the implementation of this class
//
#ifdef WIN32
class CArchitectureApp : public CWinApp
{
public:
	CArchitectureApp();
	CMultiDocTemplate* pDocTemplate;

	LPTESTCASE		m_pTestcase;			// 由测试集形成的链表
	LPTESTCASE		m_pTestHead;
	
	// 一个testcase对应一个主窗口
	CMainFrame		*m_pMain;
	CMainFrame		*m_pMainHead;

	LPMGRJOB		m_pMgrJob;
	int				m_TestIndex;

// Overrides
public:
	virtual BOOL InitInstance();
	void* doinput();
// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnParserOpenfiles();
};

extern CArchitectureApp theApp;
#else
class CArchitectureApp
{
    public:
        CArchitectureApp();
        LPTESTCASE   m_pTestcase;
        LPTESTCASE   m_pTestHead;
        LPMGRJOB     m_pMgrJob;
        int          m_TestIndex;

    public:
        void* doinput();
        void OnParserOpenfiles();

};
#endif
