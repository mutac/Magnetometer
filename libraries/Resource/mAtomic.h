
#ifndef _ATOMIC_H_1683d48d_eae8_4afe_95c9_34b6baeabffa
#define _ATOMIC_H_1683d48d_eae8_4afe_95c9_34b6baeabffa

/**
 * Default atomic type.  Operations are supported only from
 * within specializations.
 */
template <typename T>
class mAtomic
{
public:
  mAtomic()
  {
  }

  mAtomic(const T& v)
  {
  }

private:
  mAtomic(const mAtomic& other);
  mAtomic& operator=(const mAtomic&);
}; 

#include "mAtomicImpl.h"

/**
 * Atomic integer specialization
 */
template<>
class mAtomic<int> : public mAtomicImpl::mAtomicIntegralBase<int>
{
private:
  typedef mAtomicImpl::mAtomicIntegralBase<int> base;

public:
  mAtomic()
  {
  }

  mAtomic(const int& v) :
    base(v)
  {
  }

  /* default dtor OK */

private:
  mAtomic(const mAtomic& other);
  mAtomic& operator=(const mAtomic&);
};

/**
 * Atomic integer specialization
 */
template<>
class mAtomic<unsigned int> : public mAtomicImpl::mAtomicIntegralBase<unsigned int>
{
private:
  typedef mAtomicImpl::mAtomicIntegralBase<unsigned int> base;

public:
  mAtomic()
  {
  }

  mAtomic(const unsigned int& v) :
    base(v)
  {
  }

  /* default dtor OK */

private:
  mAtomic(const mAtomic& other);
  mAtomic& operator=(const mAtomic&);
};

/*
template <typename T>
class mAtomic
{
public:
  mAtomic()
  {
  }

  mAtomic(volatile const T& v)
  {
    mAtom = v;
  }

  mAtomic(const mAtomic& other)
  {
    *this = other;
  }

  // Pre-increment
  mAtomic& operator++()
  {
    ++mAtom;
    return *this;
  }

  // Post-increment
  mAtomic operator++(int)
  {
    mAtomic temp(*this);
    ++mAtom;
    return temp;
  }

  // Pre-decrement
  mAtomic& operator--()
  {
    --mAtom;
    return *this;
  }

  // Post-decrement
  mAtomic operator--(int)
  {
    mAtomic temp(*this);
    --mAtom;
    return temp;
  }

  template <typename U>
  bool operator==(const mAtomic<U>& rhs) const
  {
    return mAtom == rhs.mAtom;
  }

  template <typename U>
  bool operator==(const volatile U& rhs) const
  {
    return mAtom == rhs;
  }

  template <typename U>
  mAtomic& operator=(const mAtomic<U>& rhs)
  {
    mAtom = rhs.mAtom;
    return *this;
  }

  template <typename U>
  mAtomic& operator=(const volatile U& rhs)
  {
    mAtom = rhs;
    return *this;
  }

  operator T() const
  {
    return mAtom;
  }

  template <typename U>
  operator U() const
  {
    return (U)mAtom;
  }

  template <typename U>
  void exchange(mAtomic<T>& rhs)
  {
    mStd::swap(mAtom, rhs.mAtmoic);
  }

  template <typename U>
  void exchange(U& rhs)
  {
    mStd::swap(mAtom, rhs);
  }

private:
  T mAtom;
}; 
*/

#endif // header guard

