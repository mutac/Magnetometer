
#include "mPlatform.h"

#ifdef mPlatformAvr
#include <util/atomic.h>

namespace mPlatform
{
  template <>
  int atomic_fetch_and_add(int volatile* v, int dV)
  {
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE);
    int tmp = *v;
    *v = tmp + dV;
    return tmp;
  }

  template <>
  int atomic_fetch_and_subtract(int volatile* v, int dV)
  {
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE);
    int tmp = *v;
    *v = tmp - dV;
    return tmp;
  }

  template <>
  int atomic_add_and_fetch(int volatile* v, int dV)
  {
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE);
    return (*v = *v + dV);
  }

  template <>
  int atomic_subract_and_fetch(int volatile* v, int dV)
  {
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE);
    return (*v = *v - dV);
  }

  template <>
  int atomic_load(int const volatile* v)
  {
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE);
    return *v;
  }

  template <>
  void atomic_store(int volatile* v, int nV)
  {
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE);
    *v = nV;
  }
}
#endif // mPlatformAvr
