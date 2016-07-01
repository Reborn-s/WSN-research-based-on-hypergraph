// CArchitecture.cpp : Defines the class behaviors for the application.
/*
 * Project------ CArchitecture
 * Class Name--- CArchitecture.cpp
 * Author------- wxf891201@gmail.com
 * Date--------- 2013-4-8
 * Edition------ 1.0

 * Description-- MFC的APP, 实现同时打开指定的多个后缀名相同的文件

 * Change Log:
 *		Date-----
 *		Staff----
 *		Edition--
 *		Content--
 */


#include "Architecture.h"
#include <iostream>
#ifdef WIN32
#include "MainFrm.h"
#include "stdafx.h"
#include "ChildFrm.h"
#include "ArchitectureDoc.h"
#include "ArchitectureView.h"
#include <iostream>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CArchitectureApp

BEGIN_MESSAGE_MAP(CArchitectureApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CArchitectureApp::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinApp::OnFilePrintSetup)
	ON_COMMAND(ID_PARSER_OPENFILES, &CArchitectureApp::OnParserOpenfiles)
END_MESSAGE_MAP()


// The one and only CArchitectureApp object

CArchitectureApp theApp;


// CArchitectureApp initialization

BOOL CArchitectureApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)
	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	//CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(IDR_CArchitectureTYPE,
		RUNTIME_CLASS(CArchitectureDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CCArchitectureView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;
	// call DragAcceptFiles only if there's a suffix
	//  In an MDI app, this should occur immediately after setting m_pMainWnd


	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;
	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// The main window has been initialized, so show and update it
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	return TRUE;
}



// CAboutDlg dialog used for App About

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

// App command to run the dialog
void CArchitectureApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

#endif

CArchitectureApp::CArchitectureApp()
{

}
// CArchitectureApp message handlers
void* loopJob(void* pParam)
{
	CMgrJob* loopjob;
	loopjob = (CMgrJob*)pParam;
//	CLogNote("MGRJOB THREAD LOOPJOB");
	return loopjob->dwLoopWork();
}

void* IOThread(void* pParam)
{
	CMgrJob* IOjob;
	IOjob = (CMgrJob*)pParam;
//	CLogNote("MGRJOB THREAD LOOPJOB");
	return IOjob->dwLoopIOWork();
}

void CArchitectureApp::OnParserOpenfiles()
{
	/*
	 * 此处testcases_menu文件中每行前面加上了path: 是为了与在该行后面加上file等条目区分开，
	 * 本次测试中没有考虑分开给出测试文件名
	*/	
	fstream			pFileTxt;					// 用于操作存有所有测试文件路径的那个文本文件
	string			strFileName;				// 配置文件(包含所有测试文件的路径及文件名)
	string			testFilePath;				// 每一个测试文件的路径及
	string			testFileName;
	size_t			posPath;
	LPTESTCASE		pTestCase;
	
	fstream			pFileDir;
	string			fileName;
	string			fileNameTmp;
	string			filePath;

	vector < string >	ary_fileName;	// testcase文件名

	strFileName		= "..//testcases_menu.txt";
	testFilePath	= "";
	testFileName	= "";
	posPath			= 0;
	pTestCase		= NULL;
	fileNameTmp		= "";
	filePath		= "";
	//_CrtSetBreakAlloc(4577);
	pFileTxt.open ( strFileName.c_str() ) ;
	while ( getline ( pFileTxt, testFilePath ) )
	{
		posPath	= testFilePath.find ( "path:" );
		if ( -1 != posPath )
		{
			testFileName	= testFilePath.substr ( posPath + 5 );		// 从path:之后开始到结尾的子串均为路径名
			ary_fileName.push_back(testFileName);
			//pTestCase		= new CTestCase ( m_TestIndex, testFileName );
			//pTestCase->doParsing ( pTestCase );
			//m_pMgrJob->addJob(pTestCase);
			//m_pTestcase->doModeling ( m_pTestcase ); 

			//m_pTestcase->m_pNext_test	= m_pTestHead;
			//m_pTestHead					= m_pTestcase;

			//m_TestIndex	++;
		}		
	}
	pFileTxt.close ( );
	m_pMgrJob	= new CMgrJob ( );
	for ( int i=0; i < ary_fileName.size(); i++ )
	{
		pTestCase		= new CTestCase ( i, ary_fileName.at(i) );
		m_pMgrJob->addIOJob(pTestCase);
	}

// added by loyoen 

	CThread*			cpInputJobThread;
	CThread*			cpDealThread;
	CThread*			cpDealThread2;

	cpInputJobThread	= new CThread ( IOThread, m_pMgrJob );
	cpDealThread		= new CThread ( loopJob, m_pMgrJob );
	cpDealThread2		= new CThread ( loopJob, m_pMgrJob );
#ifndef WIN32
    cpDealThread->wait4ThisThread();
    sleep(2);
#endif

}


