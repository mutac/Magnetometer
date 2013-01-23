
#ifndef _DEFS_H_
#define _DEFS_H_

//
// Configure debugging

#ifndef mDebugging
#if defined(_DEBUG) || !defined(NDEBUG)
/** 
 * @description Enable debug and instrumented code
 */
#define mDebugging
#endif // if defined(_DEBUG) || !defined(NDEBUG)
#endif // ifndef mDebugging

// 
// Configure assertion functions
//

#include <assert.h>

/**
 * @description Assert if expression is false
 */
#define mAssert(expr) assert(expr)

#ifdef mDebugging
/**
 * @description Assert if expression is false.
 * @note Only if built for debugging.
 */
#define mDebugAssert(expr) assert(expr)
#else // ifdef mDebugging
/**
 * @description Assert if expression is false.
 * @note Only if built for debugging.
 */
#define mDebugAssert(expr)
#endif // ifdef mDebugging

//
// General purpose macros
//

/**
 * @description Suppress compiler warnings regarding variables that have been set but never used.
 * @note (void)variable does not suppress warning in some compilers.
 */
#define mUnusedVariable(var) \
  do { } while(0 && var)

/**
 * @description Returns the number of elements in a static array.
 */
#define mCountOf(staticArray) \
  (sizeof(staticArray) / sizeof(*staticArray))

#endif // Header guard
