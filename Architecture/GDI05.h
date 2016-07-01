// GDI05.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CGDI05App:
// See GDI05.cpp for the implementation of this class
//

class CGDI05App : public CWinApp
{
public:
	CGDI05App();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CGDI05App theApp;