
#ifndef _SHAREDCOUNT_H_19519545_a1e1_4a41_860d_49eab62331f0
#define _SHAREDCOUNT_H_19519545_a1e1_4a41_860d_49eab62331f0

#include "Defs.h"

typedef int AtomicCountType;

class AtomicCount
{
public:
  AtomicCount() :
    mCount(1)
  {
  }

  virtual ~AtomicCount()
  {
  }

  void addRef()
  {
    // TODO: make atomic
    ++mCount;
  }

  void release()
  {
    // TODO: make atomic
    if (--mCount == 0)
    {
      destroy();
    }
  }

  int getUseCount() const
  {
    return mCount;
  }

  void destroy()
  {
    // TODO: Deallocator
    delete this;
  }

private:
  AtomicCountType mCount;
};

// TODO: make more memory efficient:
//
// SharedCount x;
// x = y;
//
// Will allocate two AtomicCount objects.
//

typedef AtomicCountType SharedCountType;

/**
 * @brief 
 */
class SharedCount
{
public:
  SharedCount() :
    // TODO: Allocator
    mCount(new AtomicCount())
  {
  }

  SharedCount(SharedCount const & rhs)
  {
    mDebugAssert(rhs.mCount != NULL);

    AtomicCount* rhsCount = rhs.mCount;
    rhsCount->addRef();
    mCount = rhsCount;
  }

  virtual ~SharedCount()
  {
    mDebugAssert(mCount != NULL);
    mCount->release();
  }

  SharedCountType getUseCount() const
  {
    mDebugAssert(mCount != NULL);
    return mCount->getUseCount();
  }

  SharedCount& operator=(SharedCount& rhs)
  {
    mDebugAssert(rhs.mCount != NULL && mCount != NULL);

    AtomicCount* rhsCount = rhs.mCount;
    if (mCount != rhsCount)
    {
      // TODO: does this need to be done atomically?
      rhsCount->addRef();
      mCount->release();
      mCount = rhsCount;
    }

    return *this;
  }

  friend inline bool operator==(SharedCount const & lhs, 
    SharedCount const & rhs)
  {
    return lhs.mCount == rhs.mCount;
  }

  friend inline bool operator!=(SharedCount const & lhs,
    SharedCount const & rhs)
  {
    return lhs.mCount != rhs.mCount;
  }

private:
  AtomicCount* mCount;
};

#endif
