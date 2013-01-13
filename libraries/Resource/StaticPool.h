
#ifndef _STATIC_POOL_H_
#define _STATIC_POOL_H_

#include "IAllocator.h"
#include <string.h>

template <unsigned int PoolSize, class T>
class StaticPool : public IAllocator<T>
{
public:
  StaticPool() :
    mAllocated(0)
  {
  }

  T* allocate()
  {
    if (mAllocated == PoolSize)
    {
      return NULL;
    }

    return &mPool[mAllocated++];
  }
  
  void deallocate(T* t)
  {
    // Nope...
  }

  int getSize() const
  {
    return PoolSize;
  }

  int getAllocated() const
  {
    return mAllocated;
  }

private:
  T mPool[PoolSize];
  int mAllocated;
};

#endif
