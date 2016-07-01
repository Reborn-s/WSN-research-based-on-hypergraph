#include <stdio.h>
#include <iostream>
#include "Architecture.h"
#ifdef WIN32
#define sleep(x) Sleep((x)*1000)
#endif
using namespace std;

int main()
{
    CArchitectureApp *test = new CArchitectureApp();
    cout<<"start....."<<endl; 
    test->OnParserOpenfiles();
    return 0;
}
