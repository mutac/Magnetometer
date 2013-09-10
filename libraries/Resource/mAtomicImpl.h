
#ifndef _ATOMIC_IMPL_H_685c08ec_1de9_458d_ba94_cfb164ad9032
#define _ATOMIC_IMPL_H_685c08ec_1de9_458d_ba94_cfb164ad9032

// Do not include directly

#include "mPlatform.h"

/**
 * Integer specialization of mAtomic
 */
namespace mAtomicImpl
{
  template <typename T>
  class mAtomicIntegralBase
  {
  public:
    mAtomicIntegralBase()
    {
    }

    mAtomicIntegralBase(T v) :
      mVal(v)
    {
    }

    /* Default dtor OK */

    operator T() const volatile
    {
      return load();
    }

    T operator=(T v)
    {
      store(v);
      return v;
    }

    T operator++(int) volatile
    {
      return mPlatform::atomic_fetch_and_add(&mVal, 1);
    }

    T operator++() volatile
    {
      return mPlatform::atomic_add_and_fetch(&mVal, 1);
    }

    T operator--(int) volatile
    {
      return mPlatform::atomic_fetch_and_subtract(&mVal, 1);
    }

    T operator--() volatile
    {
      return mPlatform::atomic_subract_and_fetch(&mVal, 1);
    }

    T operator+=(T dV) volatile
    {
      return mPlatform::atomic_add_and_fetch(&mVal, dV);
    }

    T operator-=(T dV) volatile
    {
      return mPlatform::atomic_subract_and_fetch(&mVal, dV);
    }

    //
    //
    //

    T load() const volatile
    {
      return mPlatform::atomic_load(&mVal);
    }

    void store(T v) volatile
    {
      mPlatform::atmoic_store(&mVal, v);
    }

  private:
    T mVal;

  private:
    mAtomicIntegralBase(const mAtomicIntegralBase&);
    mAtomicIntegralBase& operator=(const mAtomicIntegralBase&);
  };
}


#endif

