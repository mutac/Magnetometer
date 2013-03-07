
#ifndef _OBJECTTRAITS_H_6b3b83a1_30e4_420a_8f27_cffe477add6a
#define _OBJECTTRAITS_H_6b3b83a1_30e4_420a_8f27_cffe477add6a

#include "mDefs.h"

// http://www.codeproject.com/Articles/4795/C-Standard-Allocator-An-Introduction-and-Implement

template<typename T>
class ObjectTraits
{
public:
  template<typename U>
  struct rebind 
  {
    typedef ObjectTraits<U> other;
  };

  inline explicit ObjectTraits() {}
  inline ~ObjectTraits() {}
  template <typename U>
  inline explicit ObjectTraits(ObjectTraits<U> const&) {}

  inline T* addressOf(T& rhs) 
  { 
    return &rhs; 
  }
  
  inline T const* addressOf(T const& rhs) 
  { 
    return &rhs; 
  }

  inline void construct(T* ptr, const T& t)
  {
    new(ptr) T(t);
  }

  inline void construct(T* ptr)
  {
    new(ptr) T;
  }

  inline void destroy(T* ptr)
  {
    ptr->~T();
  }
};

#endif // header guard
