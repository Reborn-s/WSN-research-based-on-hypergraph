//
// MATLAB Compiler: 4.14 (R2010b)
// Date: Fri Mar 04 13:26:36 2016
// Arguments: "-B" "macro_default" "-W" "cpplib:libhgraphSimulation3" "-T"
// "link:lib" "hgraphSimulation3" "-C" 
//

#include <stdio.h>
#define EXPORTING_libhgraphSimulation3 1
#include "libhgraphSimulation3.h"

static HMCRINSTANCE _mcr_inst = NULL;


#if defined( _MSC_VER) || defined(__BORLANDC__) || defined(__WATCOMC__) || defined(__LCC__)
#ifdef __LCC__
#undef EXTERN_C
#endif
#include <windows.h>

static char path_to_dll[_MAX_PATH];

BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, void *pv)
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
        if (GetModuleFileName(hInstance, path_to_dll, _MAX_PATH) == 0)
            return FALSE;
    }
    else if (dwReason == DLL_PROCESS_DETACH)
    {
    }
    return TRUE;
}
#endif
#ifdef __cplusplus
extern "C" {
#endif

static int mclDefaultPrintHandler(const char *s)
{
  return mclWrite(1 /* stdout */, s, sizeof(char)*strlen(s));
}

#ifdef __cplusplus
} /* End extern "C" block */
#endif

#ifdef __cplusplus
extern "C" {
#endif

static int mclDefaultErrorHandler(const char *s)
{
  int written = 0;
  size_t len = 0;
  len = strlen(s);
  written = mclWrite(2 /* stderr */, s, sizeof(char)*len);
  if (len > 0 && s[ len-1 ] != '\n')
    written += mclWrite(2 /* stderr */, "\n", sizeof(char));
  return written;
}

#ifdef __cplusplus
} /* End extern "C" block */
#endif

/* This symbol is defined in shared libraries. Define it here
 * (to nothing) in case this isn't a shared library. 
 */
#ifndef LIB_libhgraphSimulation3_C_API
#define LIB_libhgraphSimulation3_C_API /* No special import/export declaration */
#endif

LIB_libhgraphSimulation3_C_API 
bool MW_CALL_CONV libhgraphSimulation3InitializeWithHandlers(
    mclOutputHandlerFcn error_handler,
    mclOutputHandlerFcn print_handler)
{
    int bResult = 0;
  if (_mcr_inst != NULL)
    return true;
  if (!mclmcrInitialize())
    return false;
  if (!GetModuleFileName(GetModuleHandle("libhgraphSimulation3"), path_to_dll, _MAX_PATH))
    return false;
    bResult = mclInitializeComponentInstanceNonEmbeddedStandalone(  &_mcr_inst,
                                                                    path_to_dll,
                                                                    
                                      "libhgraphSimulation3",
                                                                    LibTarget,
                                                                    error_handler, 
                                                                    print_handler);
    if (!bResult)
    return false;
  return true;
}

LIB_libhgraphSimulation3_C_API 
bool MW_CALL_CONV libhgraphSimulation3Initialize(void)
{
  return libhgraphSimulation3InitializeWithHandlers(mclDefaultErrorHandler, 
                                                    mclDefaultPrintHandler);
}

LIB_libhgraphSimulation3_C_API 
void MW_CALL_CONV libhgraphSimulation3Terminate(void)
{
  if (_mcr_inst != NULL)
    mclTerminateInstance(&_mcr_inst);
}

LIB_libhgraphSimulation3_C_API 
long MW_CALL_CONV libhgraphSimulation3GetMcrID() 
{
  return mclGetID(_mcr_inst);
}

LIB_libhgraphSimulation3_C_API 
void MW_CALL_CONV libhgraphSimulation3PrintStackTrace(void) 
{
  char** stackTrace;
  int stackDepth = mclGetStackTrace(&stackTrace);
  int i;
  for(i=0; i<stackDepth; i++)
  {
    mclWrite(2 /* stderr */, stackTrace[i], sizeof(char)*strlen(stackTrace[i]));
    mclWrite(2 /* stderr */, "\n", sizeof(char)*strlen("\n"));
  }
  mclFreeStackTrace(&stackTrace, stackDepth);
}


LIB_libhgraphSimulation3_C_API 
bool MW_CALL_CONV mlxHgraphSimulation3(int nlhs, mxArray *plhs[], int nrhs, mxArray 
                                       *prhs[])
{
  return mclFeval(_mcr_inst, "hgraphSimulation3", nlhs, plhs, nrhs, prhs);
}

LIB_libhgraphSimulation3_CPP_API 
void MW_CALL_CONV hgraphSimulation3(int nargout, mwArray& y, const mwArray& x)
{
  mclcppMlfFeval(_mcr_inst, "hgraphSimulation3", nargout, 1, 1, &y, &x);
}
