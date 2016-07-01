// MainFrm.cpp : implementation of the CMainFrame class
//
/*
 * Project------ CArchitecture
 * Class Name--- MainFrm.cpp
 * Author------- wxf891201@gmail.com
 * Date--------- 2013-4-8
 * Edition------ 1.0

 * Description-- 对多个文档进行分析, 得到simplex，每个类间关系对应一个CSimplex对象 

 * Change Log:
 *		Date----- 2013-4-18
 *		Content-- 对多个文档分析，由内存读取改为直接Cstdio读取。并取得类的对象名，用于后续查找其他函数中是否出现该类对象

 * Change Log:
 *		Date----- 2013-5-5
 *		Content-- 修改了函数解析部分的思路，得到函数间被调用关系，即某个类中某个函数被其他哪些类中的哪些函数调用过。这种被调用关系作为类间关系的依据，并作为建立超边的依据

 * Change Log:
 *		Date----- 2013-5-6
 *		Content-- 开始实现超图建模，分别由类得到顶点及类间关系得到超边
*/

#include "stdafx.h"
#include "Architecture.h"

#include "MainFrm.h"

#include "Hypergraph.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	ON_WM_CREATE()
	
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	
}

CMainFrame::~CMainFrame()
{
	//LPCLASS	pClass;
	//pClass		= m_pHeadCl;
	//while ( pClass )
	//{
	//	m_pHeadCl	= pClass->cpNext;
	//	delete	pClass;
	//	pClass	= m_pHeadCl;
	//}

	//LPSIMPLEX	pSimplex;
	//pSimplex	= m_psimHead;
	//while ( pSimplex )
	//{
	//	m_psimHead	= pSimplex->pNext;
	//	delete	pSimplex;
	//	pSimplex	= m_psimHead;
	//}

	//LPRELATION	pRelation;
	//pRelation	= m_pReHead;
	//while ( pRelation )
	//{
	//	m_pReHead	= pRelation->pNextRelation;
	//	delete	pRelation->pCls;
	//	//还有一层, relation中new出来的class 要释放
	//	delete	pRelation;
	//	pRelation	= m_pReHead;
	//}
	//
	// 这里需要处理函数关系链的内存释放
	/*LPMETHOD	pMethodDoc;
	LPMETHOD	pMethod;
	LPMETHOD	pMethodIvk;

	pMethodDoc	= m_pHeadMth;
	while ( pMethodDoc )
	{
		pMethod	= pMethodDoc;
		while ( pMethod )
		{
			pMethodIvk	= pMethod;
			while ( pMethodIvk )
			{
				m_pHeadMth	= pMethodIvk->mthNextOnIvk;
				delete		pMethodIvk;
				pMethodIvk	= m_pHeadMth;
			}
			m_pHeadMth	= pMethod->mthpNext;
			delete		pMethod;
			pMethod		= m_pHeadMth;
		}
		m_pHeadMth	= pMethodDoc->mthNextOnDoc;
		delete		pMethodDoc;
		pMethodDoc	= m_pHeadMth;
	}*/

}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}


// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG
