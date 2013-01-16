
#ifndef _DIAGNOSTICS_H_
#define _DIAGNOSTICS_H_

#ifdef mDebugging
#include <assert.h>
/**
 */
#define mDiag_DebugAssert(expr) do{ assert(expr); }while(0)
#else
/**
 */
#define mDiag_DebugAssert(expr) 
#endif // ifdef mDebugging

#endif // Header guard