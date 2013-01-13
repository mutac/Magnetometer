
#ifndef _I_ALLOCATOR_H_
#define _I_ALLOCATOR_H_

template <class T>
class IAllocator
{
public:
  virtual T* allocate() = 0;
  virtual void deallocate(T* t) = 0;
};

#endif
