// GDI05Dlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include "Resource.h"
#include <vector>
#include "Architecture.h"
using namespace std;

// CGDI05Dlg dialog
class CGDI05Dlg : public CDialog
{
// Construction
public:
	CGDI05Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_GDI05_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl m_tree;
	ULONG_PTR gdiplusToken;
	void StartTest();
	void DrawTree(vector<int> Parts,vector<string> Nodes,vector<double> ModuList,vector<double> CohenList, string outFoldPath);
	vector<double> MapSizeofList(vector<double> onelist,int MaxSize);

	CArchitectureApp *test;
	CThread *doThread;
	afx_msg void OnBnClickedCancel();
};
struct DataPass
{
	void*			  AppTest;
	CDialog*		  thisDlg;
};

