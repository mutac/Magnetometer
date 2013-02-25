
#ifndef _IALLOCATOR_H_712b764c_c6dc_418d_9a82_0838b9e6e5b7
#define _IALLOCATOR_H_712b764c_c6dc_418d_9a82_0838b9e6e5b7

#include "mDefs.h"
#include <string.h>

/**
 * @description A very simple allocator interface
 */
template <class T>
class IAllocator
{
public:
  virtual T* allocate() = 0;
  virtual void deallocate(T* t) = 0;
};

/**
 * @description Generic allocator that does nothing. Nice 
 *  for default constructors and testing.
 */
template <class T>
class NullAllocator : public IAllocator<T>
{
public:
  T* allocate() { return NULL; }
  void deallocate(T*) { }

  /**
   * @note There only needs to be one of these.
   */
  static NullAllocator<T>& instance()
  {
    // This is tricky.  Not all compilers support
    // method-static variables within a template. 
    // And others don't fully support template 
    // specialization... so...
    // 
    static NullAllocator<T> sAllocator;
    return sAllocator;
  }
};

#endif
