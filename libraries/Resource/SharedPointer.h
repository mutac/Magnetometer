
#ifndef _SHAREDPOINTER_H_02b6f995_7ac4_42db_91b1_c94199cef10c
#define _SHAREDPOINTER_H_02b6f995_7ac4_42db_91b1_c94199cef10c

#include "Defs.h"
#include "SharedCount.h"

// TODO: make more memory efficient:
//
// SharedPointer<Type> x;
// x = yPtr;
//
// Will allocate two SharedCount objects.
//

template<class T>
class SharedPointer
{
public:
  SharedPointer() :
    mShared(NULL)
  {
  }

  SharedPointer(T* ptr) :
    mShared(ptr)
  {
  }

  SharedPointer(SharedPointer<T> const & rhs) :
    // TODO: Do these two need to be done atomically?
    mCount(rhs.mCount),
    mShared(rhs.mShared)
  {
  }

  virtual ~SharedPointer()
  {
    release();
  }

  inline T* get() const
  {
    return mShared;
  }

  inline T& operator*() const
  {
    mDebugAssert(mShared != NULL);
    return *mShared;
  }

  inline T* operator->() const
  {
    mDebugAssert(mShared != NULL);
    return mShared;
  }

  SharedPointer<T>& operator=(SharedPointer<T> const & rhs)
  {
    SharedCount rhsCount(rhs.mCount);
    T* rhsShared = rhs.mShared;

    if (mCount != rhsCount)
    {
      release();
      mCount = rhsCount;
      mShared = rhsShared;
    }

    return *this;
  }

private:
  /**
   * @note Warning: Caller must release mCount
   */
  void release()
  {
    if (mCount.getUseCount() == 1)
    {
      // TODO: Allocator/Deallocator
      if (mShared != NULL)
      {
        delete mShared;
      }
    }
  }

  T* mShared;
  SharedCount mCount;
};

template<class T, class U>
inline bool operator==(SharedPointer<T> const & rhs, SharedPointer<U> const & lhs)
{
  return rhs.get() == lhs.get();
}

template<class T, class U>
inline bool operator!=(SharedPointer<T> const & rhs, SharedPointer<U> const & lhs)
{
  return rhs.get() != lhs.get();
}

#endif