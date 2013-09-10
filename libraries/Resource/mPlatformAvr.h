#ifndef _PLATFORM_AVR_H_49f11cf9_9e43_4cfa_8cc1_84672829f012
#define _PLATFORM_AVR_H_49f11cf9_9e43_4cfa_8cc1_84672829f012

// Do not include directly

#ifdef mPlatformAvr
namespace mPlatform
{
  /** 
   * Atomic operations over int type
   */
  template <>
  int atomic_fetch_and_add(int volatile* v, int dV);
  template <>
  int atomic_fetch_and_subtract(int volatile* v, int dV);
  template <>
  int atomic_add_and_fetch(int volatile* v, int dV);
  template <>
  int atomic_subract_and_fetch(int volatile* v, int dV);
}
#endif // mPlatformAvr

#endif // Header guard