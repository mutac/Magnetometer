
#ifndef _SHAREDCOUNT_H_19519545_a1e1_4a41_860d_49eab62331f0
#define _SHAREDCOUNT_H_19519545_a1e1_4a41_860d_49eab62331f0

#include "mDefs.h"

/**
 * @brief Type used to count references
 */
typedef int AtomicCountType;

/**
 * @brief A platform specific implementation of an atomic counter
 */
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

/**
 * @brief Type used to count references
 */
typedef AtomicCountType SharedCountType;

/**
 * @brief A shared counter.  Used to implement reference counting.
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

  inline SharedCountType getUseCount() const
  {
    // Empty counts as a ref of 1
    return mCount != NULL ? mCount->getUseCount() : 1;
  }

  inline bool empty() const
  {
    return mCount == NULL;
  }

  SharedCount& operator=(SharedCount const & rhs)
  {
    //
    // TODO: does this need to be done atomically?
    //

    if (*this != rhs || empty())
    {
      // TODO: Is this ordered correctly?
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
    if (mCount == NULL)
    {
      // TODO: Allocator/Deallocator
      mCount = new AtomicCount();
    }
    mCount->addRef();
  }

  /**
   * @brief Releases a count.  Counter will be empty afterwards.
   */
  void release() const
  {
    if (mCount != NULL)
    {
      mCount->release();
      mCount = NULL;
    }
  }

  // Cheating: 
  //
  // Pretend that adding or releasing a reference
  // is a const operation.  This allows counters to be 
  // allocated as needed, when a reference is made.  At that
  // time both the left hand and right hand sides need to be
  // updated, through a const operations of assignment and
  // copy construction.
  mutable AtomicCount* mCount;
};

#endif
