// GDI05Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "GDI05.h"
#include "GDI05Dlg.h"
#include <GdiPlus.h>
#include <math.h>
#include <iostream>
#include <fstream>
/*
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
*/

// CAboutDlg dialog used for App About
#pragma comment(lib, "..//gdiplus//lib//gdiplus.lib")
using namespace Gdiplus;


class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CGDI05Dlg dialog




CGDI05Dlg::CGDI05Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGDI05Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGDI05Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE, m_tree);
}

BEGIN_MESSAGE_MAP(CGDI05Dlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDCANCEL, &CGDI05Dlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CGDI05Dlg message handlers

BOOL CGDI05Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	StartTest();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_tree.ModifyStyle(NULL,TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT);
	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CGDI05Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CGDI05Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CGDI05Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CGDI05Dlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	//OnCancel();
	GdiplusShutdown(gdiplusToken);
	//delete doThread;
	if(test!=NULL)
	{
		test->stopThreads();
		Sleep(2000);
		delete doThread;
		delete test;
		test = NULL;
	}
}
void doThreadFun(void *para)
{
	DataPass *tmpData = (DataPass*)para;
	CArchitectureApp *test = (CArchitectureApp*)tmpData->AppTest;
	test->OnParserOpenfiles(tmpData->thisDlg);
}
vector<double> CGDI05Dlg::MapSizeofList(vector<double> onelist,int MaxSize)
{
	double MaxElement = 0.0;
	for(int i=0;i<onelist.size();i++)
		MaxElement = MaxElement>onelist[i]?MaxElement:onelist[i];
	vector<double> anslist;
	for(int i=0;i<onelist.size();i++)
	{
		double thisans = onelist[i]*double(MaxSize)/MaxElement;
		anslist.push_back(thisans);
	}
	return anslist;
}


void CGDI05Dlg::DrawTree(vector<int> Parts,vector<string> Nodes,vector<double> ModuList,vector<double> CohenList, string outFoldPath)
{
	int maxPart = 0;
	
	for(int i=0;i<Parts.size();i++)
	{
		maxPart = maxPart>Parts[i]?maxPart:Parts[i];
	}
	maxPart += 1;
	int *NumNodesInPart = (int*)malloc(sizeof(int)*maxPart);
	HTREEITEM *hItem = new HTREEITEM[maxPart];
	//NumNodesInPart = new int[maxPart];
	memset(NumNodesInPart,0,maxPart*sizeof(int));
	for(int i=0;i<maxPart;i++)
	{
		char numstr[10];
		sprintf(numstr,"%d",i);
		hItem[i] = m_tree.InsertItem(CString(numstr),NULL,NULL);
	}
	for(int i=0;i<Nodes.size();i++)
	{
		HTREEITEM hSubItem = m_tree.InsertItem(CString(Nodes[i].c_str()),NULL,NULL,hItem[Parts[i]]);
		NumNodesInPart[Parts[i]]++;
	}
	/*double R = 280;
	int OX = 400;
	int OY = 400;*/
	double R = 120;
	int OX = 815;
	int OY = 200;
	double PI = 3.1416;
	int MaxNum = 0;
	for(int i=0;i<maxPart;i++)
	{
		NumNodesInPart[i] = int(sqrt(double(NumNodesInPart[i])));
		MaxNum = MaxNum>NumNodesInPart[i]?MaxNum:NumNodesInPart[i];
	}
	for(int i=0;i<maxPart;i++)
	{
		/*NumNodesInPart[i] = int(100.0*double(NumNodesInPart[i])/double(MaxNum));*/
		NumNodesInPart[i] = int(50.0*double(NumNodesInPart[i])/double(MaxNum));
	}

	//vector<int> ModuMapList = MapSizeofList(ModuList,10);
	vector<double> ModuMapList = MapSizeofList(ModuList,5);
	vector<double> CohenMapList = MapSizeofList(CohenList,200);
	CClientDC dc(this);
	Graphics graphics(dc);

	// »­ñîºÏ²¿·Ö
	int Cnt = 0;
	for(int i=0;i<maxPart;i++)
	{
		int StartX = OX + int(R*cos(2.0*PI/double(maxPart)*i));
		int StartY = OY + int(R*sin(2.0*PI/double(maxPart)*i));
	
		for(int j=i+1;j<maxPart;j++)
		{
			int EndX = OX + int(R*cos(2.0*PI/double(maxPart)*j)) ;
			int EndY = OY + int(R*sin(2.0*PI/double(maxPart)*j)) ;
			if(ModuMapList[Cnt]!=0)
				graphics.DrawLine(&Pen(Color(0,0,0), ModuMapList[Cnt]), Point(StartX,StartY), Point(EndX,EndY));
			Cnt++;
		}
	}

	for(int i=0;i<maxPart;i++)
	{
		int x = OX + int(R*cos(2.0*PI/double(maxPart)*i));
		int y = OY + int(R*sin(2.0*PI/double(maxPart)*i));
		BYTE b = 200-CohenMapList[i];
		BYTE g = 200-CohenMapList[i];
		BYTE r = 200-CohenMapList[i];
		graphics.DrawArc(&Pen(Color(r,g,b), 1),x-NumNodesInPart[i]/2, y-NumNodesInPart[i]/2,NumNodesInPart[i],NumNodesInPart[i],0,360);
		SolidBrush redBrush(Color(r,g,b));
		graphics.FillEllipse(&redBrush, x-NumNodesInPart[i]/2, y-NumNodesInPart[i]/2, NumNodesInPart[i], NumNodesInPart[i]);

		int StartX = OX + int((R+NumNodesInPart[i])*cos(2.0*PI/double(maxPart)*i));
		int StartY = OY + int((R+NumNodesInPart[i])*sin(2.0*PI/double(maxPart)*i));
		Gdiplus::Font *newFont = new Gdiplus::Font(CString("Arial"),10);
		PointF testPoint(StartX,StartY);
		char numStr[10] = {0};
		sprintf(numStr,"%d",i);
		graphics.DrawString(CString(numStr),strlen(numStr),newFont,testPoint,new SolidBrush(Color(0,0,0)));
	}

	/*
	for(int i=0;i<maxPart;i++)
	{
		int StartX = OX + int(R*cos(2.0*PI/double(maxPart)*i)) + NumNodesInPart[i]/2;
		int StartY = OY + int(R*sin(2.0*PI/double(maxPart)*i)) + NumNodesInPart[i]/2;
		Gdiplus::Font *newFont = new Gdiplus::Font(CString("Arial"),10);
		PointF testPoint(StartX-4,StartY-5);
		char numStr[10] = {0};
		sprintf(numStr,"%d",i);
		graphics.DrawString(CString(numStr),strlen(numStr),newFont,testPoint,new SolidBrush(Color::Red));
	}
	*/

	//vector<double> ModuWriteList = MapSizeofList(ModuList,1);
	//vector<double> CohenWriteList = MapSizeofList(CohenList,1);
	ofstream ansfile, modufile;
	ansfile.open(string(outFoldPath+"//cohen.txt").c_str());
	for(int i=0;i<CohenList.size();i++)
	{
		char sValue[128] = {0};
		sprintf(sValue,"%4f",CohenList[i]);
		ansfile.write(sValue,strlen(sValue));
		ansfile.write("\n",1);
	}
	ansfile.close();

	modufile.open(string(outFoldPath+"//comp.txt").c_str());
	Cnt = 0;
	for(int i=0;i<maxPart;i++)
	{
		for(int j=i+1;j<maxPart;j++)
		{
			char sNode1[32] = {0},sNode2[32]={0},sValue[128]={0};
			sprintf(sNode1,"%d",i);
			sprintf(sNode2,"%d",j);
			sprintf(sValue,"%4f",ModuList[Cnt]);
			string line = string(sNode1)+string("\t")+string(sNode2)+string("\t")+string(sValue)+string("\n");
			modufile.write(line.c_str(),line.length());
			
			Cnt++;
		}
	}
	modufile.close();

	delete hItem;
	//free(NumNodesInPart);
}
void CGDI05Dlg::StartTest()
{
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	test = new CArchitectureApp();

	DataPass *tmpData = (DataPass*)malloc(sizeof(DataPass));
	tmpData->AppTest = test;
	tmpData->thisDlg = this;
    doThread = new CThread(doThreadFun,tmpData);
}
