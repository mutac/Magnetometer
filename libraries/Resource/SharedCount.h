
#ifndef _SHAREDCOUNT_H_19519545_a1e1_4a41_860d_49eab62331f0
#define _SHAREDCOUNT_H_19519545_a1e1_4a41_860d_49eab62331f0

#include "mDefs.h"
#include "mAtomic.h"

/**
 * @brief Type used to count references
 */
typedef unsigned int RefCountType;

/**
 * @brief Type used to count references atomically
 */
//typedef mAtomic<RefCountType> AtomicRefCount;
typedef RefCountType AtomicRefCount;

/**
 * @brief A shared counter.  Used to implement reference counting.  On construction, the count is 1.
 * @description Optimized to not allocate a counter until a reference
 *              is made.
 */
class SharedCount
{
public:
  SharedCount() :
    mCount(NULL)
  {
  }

  SharedCount(SharedCount const & rhs)
  {
    rhs.addRef();
    mCount = rhs.mCount;
  }

  virtual ~SharedCount()
  {
    release();
  }

  inline RefCountType getRefCount() const
  {
    // Empty counts as a ref of 1
    return mCount != NULL ? *mCount : 1;
  }

  inline bool empty() const
  {
    return mCount == NULL;
  }

  SharedCount& operator=(SharedCount const & rhs)
  {
    if (empty() || *this != rhs)
    {
      rhs.addRef();
      release();
      mCount = rhs.mCount;
    }

    return *this;
  }

  /**
   */
  friend inline bool operator==(SharedCount const & lhs, 
    SharedCount const & rhs)
  {
    return lhs.mCount == rhs.mCount;
  }

  /**
   */
  friend inline bool operator!=(SharedCount const & lhs,
    SharedCount const & rhs)
  {
    // If it is useful to have empty counts appear as unequal, 
    // 'this' could be used as an identifier.
    return lhs.mCount != rhs.mCount;
  }

private:
  /**
   * @brief Adds a reference count, allocates counter on first reference.
   */
  void addRef() const
  {
    // TODO: This isn't thread safe:

    if (mCount == NULL)
    {
      mCount = new AtomicRefCount(1);
    }
    
    ++(*mCount);
  }

  /**
   * @brief Releases a count.  Counter will be empty afterwards.
   */
  void release() const
  {
    // TODO: This isn't thread safe.

    if (mCount != NULL)
    {
      RefCountType refCount = --(*mCount);
      if (refCount == 0)
      {
        delete mCount;
        mCount = NULL;
      }
    }
  }

  // Mutation: 
  //
  // Pretend that adding or releasing a reference
  // is a const operation.  This allows counters to be 
  // allocated as needed, when a reference is made.  At that
  // time both the left hand and right hand sides need to be
  // updated, through a const operations of assignment and
  // copy construction.
  mutable AtomicRefCount* mCount;
};

#endif
