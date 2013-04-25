
#include "mNew.h"

#ifndef mHasNew
void* operator new(size_t size)
{
  return malloc(size);
}
#endif // !defined(mHasNew)

#ifndef mHasArrayNew
void* operator new[](size_t size)
{
  return malloc(size);
}
#endif // !defined(mHasArrayNew)

#ifndef mHasPlacementNew
void* operator new(size_t size, void* ptr)
{
  return ptr;
}
#endif // !defined(mHasNew)

#ifndef mHasDelete
void operator delete(void* ptr)
{
  free(ptr);
}
#endif // !defined(mHasDelete)

#ifndef mHasArrayDelete
void operator delete[](void* ptr)
{
  free(ptr);
}
#endif // !defined(mHasArrayDelete)