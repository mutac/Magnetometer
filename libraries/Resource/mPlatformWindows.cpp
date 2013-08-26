
#include "mPlatform.h"

#ifdef mPlatformWindows

namespace mPlatform
{
  template <>
  int atomic_fetch_and_add(int volatile* v, int dV)
  {
    return 0;
  }

  template <>
  int atomic_fetch_and_subtract(int volatile* v, int dV)
  {
    return 0;
  }

  template <>
  int atomic_add_and_fetch(int volatile* v, int dV)
  {
    return 0;
  }

  template <>
  int atomic_subract_and_fetch(int volatile* v, int dV)
  {
    return 0;
  }
}

#endif // mPlatformWindows