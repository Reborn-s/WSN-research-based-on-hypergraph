// ArchitectureView.h : interface of the CCArchitectureView class
//


#pragma once


class CCArchitectureView : public CView
{
protected: // create from serialization only
	CCArchitectureView();
	DECLARE_DYNCREATE(CCArchitectureView)

// Attributes
public:
	CArchitectureDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual void OnInitialUpdate ();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CCArchitectureView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in CArchitectureView.cpp
inline CArchitectureDoc* CCArchitectureView::GetDocument() const
   { return reinterpret_cast<CArchitectureDoc*>(m_pDocument); }
#endif

