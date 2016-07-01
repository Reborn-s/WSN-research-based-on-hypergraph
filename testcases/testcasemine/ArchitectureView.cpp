// CArchitectureView.cpp : implementation of the CCArchitectureView class
//

#include "stdafx.h"
#include "Architecture.h"

#include "ArchitectureDoc.h"
#include "ArchitectureView.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCArchitectureView

IMPLEMENT_DYNCREATE(CCArchitectureView, CView)

BEGIN_MESSAGE_MAP(CCArchitectureView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CCArchitectureView construction/destruction

CCArchitectureView::CCArchitectureView()
{
	// TODO: add construction code here

}

CCArchitectureView::~CCArchitectureView()
{
}

BOOL CCArchitectureView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

void CCArchitectureView::OnInitialUpdate()
{
	/*
	* ¶àÎÄµµ£¬´æ´¢¡£
	* 
	*/
	CArchitectureDoc	*pDocFile;
	//CMainFrame			*pMain;

	pDocFile			= (CArchitectureDoc *) GetDocument();
	ASSERT_VALID ( pDocFile );

	if ( !pDocFile )
		return;
	/*pMain	= ( CMainFrame * )this->GetParentFrame()->GetParentFrame();
	pMain->m_pVectorDocFile.push_back ( this->GetDocument() );*/
}
// CCArchitectureView drawing

void CCArchitectureView::OnDraw(CDC* /*pDC*/)
{
	CArchitectureDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CCArchitectureView printing

BOOL CCArchitectureView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CCArchitectureView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CCArchitectureView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CCArchitectureView diagnostics

#ifdef _DEBUG
void CCArchitectureView::AssertValid() const
{
	CView::AssertValid();
}

void CCArchitectureView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CArchitectureDoc* CCArchitectureView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CArchitectureDoc)));
	return (CArchitectureDoc*)m_pDocument;
}
#endif //_DEBUG


// CCArchitectureView message handlers
