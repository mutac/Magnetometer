
#include "mPlatform.h"

#ifdef mPlatformWindows
#include <windows.h>

namespace mPlatform
{
  mCompilerAssert(LongIntegerTypeSizesMatch, sizeof(LONG) == sizeof(int));

  template <>
  int atomic_fetch_and_add(int volatile* v, int dV)
  {
    mAssert((INT_PTR)v % 4 == 0);
    return InterlockedExchangeAdd((volatile LONG*)v, dV);
  }

  template <>
  int atomic_fetch_and_subtract(int volatile* v, int dV)
  {
    mAssert((INT_PTR)v % 4 == 0);
    return InterlockedExchangeAdd((volatile LONG*)v, -dV);
  }

  template <>
  int atomic_add_and_fetch(int volatile* v, int dV)
  {
    mAssert((INT_PTR)v % 4 == 0);
    return InterlockedExchangeAdd((volatile LONG*)v, dV) + dV;
  }

  template <>
  int atomic_subract_and_fetch(int volatile* v, int dV)
  {
    mAssert((INT_PTR)v % 4 == 0);
    return InterlockedExchangeAdd((volatile LONG*)v, -dV) - dV;
  }
  
  template <>
  int atomic_load(int const volatile* v)
  {
    mAssert((INT_PTR)v % 4 == 0);
    return *v;
  }

  template <>
  void atomic_store(int volatile* v, int nV)
  {
    mAssert((INT_PTR)v % 4 == 0);
    *v = nV;
  }
}

#endif // mPlatformWindows
