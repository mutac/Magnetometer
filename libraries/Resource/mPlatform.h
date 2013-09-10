/**
 */
#ifndef _PLATFORM_H_89cafc3a_6a10_4538_bf26_2b38d67ddad5
#define _PLATFORM_H_89cafc3a_6a10_4538_bf26_2b38d67ddad5

#include "mDefs.h"

/**
 * Platform specific operations
 */
namespace mPlatform
{
  template <typename T>
  T atomic_fetch_and_add(T volatile* v, T dV);

  template <typename T>
  T atomic_fetch_and_subtract(T volatile* v, T dV);

  template <typename T>
  T atomic_add_and_fetch(T volatile* v, T dV);

  template <typename T>
  T atomic_subract_and_fetch(T volatile* v, T dV);

  template <typename T>
  T atomic_load(T const volatile* v);

  template <typename T>
  void atomic_store(T volatile* v, T nV);
}

#if defined(mPlatformWindows)
  #include "mPlatformWindows.h"
#else
  #error Unknown platform...
#endif

#endif