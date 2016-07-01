//
// MATLAB Compiler: 4.14 (R2010b)
// Date: Fri Mar 04 13:26:36 2016
// Arguments: "-B" "macro_default" "-W" "cpplib:libhgraphSimulation3" "-T"
// "link:lib" "hgraphSimulation3" "-C" 
//

#ifndef __libhgraphSimulation3_h
#define __libhgraphSimulation3_h 1

#if defined(__cplusplus) && !defined(mclmcrrt_h) && defined(__linux__)
#  pragma implementation "mclmcrrt.h"
#endif
#include "mclmcrrt.h"
#include "mclcppclass.h"
#ifdef __cplusplus
extern "C" {
#endif

#if defined(__SUNPRO_CC)
/* Solaris shared libraries use __global, rather than mapfiles
 * to define the API exported from a shared library. __global is
 * only necessary when building the library -- files including
 * this header file to use the library do not need the __global
 * declaration; hence the EXPORTING_<library> logic.
 */

#ifdef EXPORTING_libhgraphSimulation3
#define PUBLIC_libhgraphSimulation3_C_API __global
#else
#define PUBLIC_libhgraphSimulation3_C_API /* No import statement needed. */
#endif

#define LIB_libhgraphSimulation3_C_API PUBLIC_libhgraphSimulation3_C_API

#elif defined(_HPUX_SOURCE)

#ifdef EXPORTING_libhgraphSimulation3
#define PUBLIC_libhgraphSimulation3_C_API __declspec(dllexport)
#else
#define PUBLIC_libhgraphSimulation3_C_API __declspec(dllimport)
#endif

#define LIB_libhgraphSimulation3_C_API PUBLIC_libhgraphSimulation3_C_API


#else

#define LIB_libhgraphSimulation3_C_API

#endif

/* This symbol is defined in shared libraries. Define it here
 * (to nothing) in case this isn't a shared library. 
 */
#ifndef LIB_libhgraphSimulation3_C_API 
#define LIB_libhgraphSimulation3_C_API /* No special import/export declaration */
#endif

extern LIB_libhgraphSimulation3_C_API 
bool MW_CALL_CONV libhgraphSimulation3InitializeWithHandlers(
       mclOutputHandlerFcn error_handler, 
       mclOutputHandlerFcn print_handler);

extern LIB_libhgraphSimulation3_C_API 
bool MW_CALL_CONV libhgraphSimulation3Initialize(void);

extern LIB_libhgraphSimulation3_C_API 
void MW_CALL_CONV libhgraphSimulation3Terminate(void);



extern LIB_libhgraphSimulation3_C_API 
void MW_CALL_CONV libhgraphSimulation3PrintStackTrace(void);

extern LIB_libhgraphSimulation3_C_API 
bool MW_CALL_CONV mlxHgraphSimulation3(int nlhs, mxArray *plhs[], int nrhs, mxArray 
                                       *prhs[]);

extern LIB_libhgraphSimulation3_C_API 
long MW_CALL_CONV libhgraphSimulation3GetMcrID();


#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

/* On Windows, use __declspec to control the exported API */
#if defined(_MSC_VER) || defined(__BORLANDC__)

#ifdef EXPORTING_libhgraphSimulation3
#define PUBLIC_libhgraphSimulation3_CPP_API __declspec(dllexport)
#else
#define PUBLIC_libhgraphSimulation3_CPP_API __declspec(dllimport)
#endif

#define LIB_libhgraphSimulation3_CPP_API PUBLIC_libhgraphSimulation3_CPP_API

#else

#if !defined(LIB_libhgraphSimulation3_CPP_API)
#if defined(LIB_libhgraphSimulation3_C_API)
#define LIB_libhgraphSimulation3_CPP_API LIB_libhgraphSimulation3_C_API
#else
#define LIB_libhgraphSimulation3_CPP_API /* empty! */ 
#endif
#endif

#endif

extern LIB_libhgraphSimulation3_CPP_API void MW_CALL_CONV hgraphSimulation3(int nargout, mwArray& y, const mwArray& x);

#endif
#endif
