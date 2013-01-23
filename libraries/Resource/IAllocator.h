
#ifndef _IALLOCATOR_H_712b764c_c6dc_418d_9a82_0838b9e6e5b7
#define _IALLOCATOR_H_712b764c_c6dc_418d_9a82_0838b9e6e5b7

#include "Defs.h"

template <class T>
class IAllocator
{
public:
  virtual T* allocate() = 0;
  virtual void deallocate(T* t) = 0;
};

#endif
