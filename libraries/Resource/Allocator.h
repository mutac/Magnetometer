
#ifndef _ALLOCATOR_H_b1f38dd0_18df_4575_ab94_ac83b9ea3521
#define _ALLOCATOR_H_b1f38dd0_18df_4575_ab94_ac83b9ea3521

#include "ObjectTraits.h"
#include "AllocationPolicy.h"

template<
  typename T, 
  typename Policy = DynamicAllocationPolicy<T>,
  typename Traits = ObjectTraits<T> >
class Allocator : public Policy, public Traits
{
private:
  typedef Policy AllocationPolicy;
  typedef Traits ObjTraits;

public:
  typedef typename AllocationPolicy::ValueType ValueType;
  typedef typename AllocationPolicy::Pointer Pointer;
  typedef typename AllocationPolicy::ConstPointer ConstPointer;
  typedef typename AllocationPolicy::Reference Reference;
  typedef typename AllocationPolicy::ConstReference ConstReference;
  typedef typename AllocationPolicy::SizeType SizeType;

  template<typename U>
  struct rebind
  {
  private:
    typedef typename AllocationPolicy::template rebind<U>::other otherPolicy;
    typedef typename ObjTraits::template rebind<U>::other otherTraits;
  public:
    typedef Allocator<U, otherPolicy, otherTraits> other;
  };

  inline explicit Allocator() {}
  inline ~Allocator() {}
  
  inline Allocator(Allocator const& rhs) :
    Policy(rhs), Traits(rhs)
  {
  }
  
  template<typename U>
  inline Allocator(Allocator<U> const&) {}

  template<typename U, typename P, typename OT>
  inline Allocator(Allocator<U, P, OT> const& rhs) :
    Policy(rhs), Traits(rhs)
  {
  }
};

#endif // header guard
