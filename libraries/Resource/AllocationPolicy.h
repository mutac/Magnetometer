
#ifndef _ALLOCATIONPOLICY_H_7fb81e52_4137_46dc_a4f0_5c629dc52d37
#define _ALLOCATIONPOLICY_H_7fb81e52_4137_46dc_a4f0_5c629dc52d37

#include "mDefs.h"

template<typename T>
class DynamicAllocationPolicy
{
public:

  typedef T ValueType;
  typedef ValueType* Pointer;
  typedef const ValueType* ConstPointer;
  typedef ValueType& Reference;
  typedef const ValueType& ConstReference;
  typedef size_t SizeType;

  template<typename U>
  struct rebind
  {
    typedef DynamicAllocationPolicy<U> other;
  };

  inline explicit DynamicAllocationPolicy() {}
  inline ~DynamicAllocationPolicy() {}
  inline explicit DynamicAllocationPolicy(DynamicAllocationPolicy const&) {}
  template<typename U>
  inline explicit DynamicAllocationPolicy(DynamicAllocationPolicy<U> const&) {}

  inline Pointer allocate(SizeType count)
  {
    return reinterpret_cast<Pointer>(::operator new(count * sizeof(T)));
  }

  inline void deallocate(Pointer ptr, SizeType)
  {
    ::operator delete(ptr);
  }
};

/**
 * @description Determines if memory from allocator type T can
 *              be freed by allocatored type U.
 */
template<typename T, typename U>
inline bool operator==(DynamicAllocationPolicy<T> const&, 
                  DynamicAllocationPolicy<U> const&)
{
  return true;
}

template<typename T, typename Other>
inline bool operator==(DynamicAllocationPolicy<T> const&,
                  Other const&)
{
  return false;
}

#endif // header guard
