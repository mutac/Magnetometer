
#ifndef _IALLOCATOR_H_712b764c_c6dc_418d_9a82_0838b9e6e5b7
#define _IALLOCATOR_H_712b764c_c6dc_418d_9a82_0838b9e6e5b7

#include "Defs.h"
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
  static NullAllocator<T>& instance();
};

#endif
