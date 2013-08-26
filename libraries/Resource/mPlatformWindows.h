#ifndef _PLATFORM_WINDOWS_H_8d892d16_a062_45a2_ac65_d34974ad475d
#define _PLATFORM_WINDOWS_H_8d892d16_a062_45a2_ac65_d34974ad475d

// Do not include directly

#ifdef mPlatformWindows
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
#endif // mPlatformWindows

#endif // Header guard
