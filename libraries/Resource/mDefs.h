/**
 * @file Defs.h
 * @description Common macros, definitions and configurations
 *
 * TODO:
 *  * Add namespaces
 *  * Specialize inclusions and namespace usage if compiling for arduino
 *
 */
#ifndef _DEFS_H_9ce33353_44da_40a5_bb3b_de0f9b3dad12
#define _DEFS_H_9ce33353_44da_40a5_bb3b_de0f9b3dad12

#include <stdlib.h>
#include <assert.h>

//
// Exception use
//

/**
 * Enable exceptions
 */
// #define mUseExceptions

//
// STL use
//

/** 
 * @description Configure STL usage
 */
// #define mUseStl

//
// RTTI
//

/**
 * @description Set if compiling with RTTI
 */
// #define mUseRtti

//
// Configure debugging
//

#ifndef mDebugging
#if defined(_DEBUG) || !defined(NDEBUG)
/** 
 * @description Enable debug and instrumented code
 */
#define mDebugging
#endif // if defined(_DEBUG) || !defined(NDEBUG)
#endif // ifndef mDebugging

// 
// Configure assertion macros
//

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
