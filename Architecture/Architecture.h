// Architecture.h : main header file for the CArchitecture application


#include "TestCase.h"
#include "CMgrJob.h"

// CArchitectureApp:
// See CArchitecture.cpp for the implementation of this class
//

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
		void stopThreads();

};

