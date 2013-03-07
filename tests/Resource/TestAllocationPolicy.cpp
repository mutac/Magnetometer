#include <cfixcc.h>
#include "AllocationPolicy.h"

class TestAllocationPolicy : public cfixcc::TestFixture
{
private:
  class DummyClass
  {
  public:
    DummyClass()
    {
    }
  private:
    char mBigSize[1000];
  };

  template<template<typename T> class Policy>
  class AllocationPolicyTests
  {
  public:
    template<typename T>
    void allocDealloc()
    {
      Policy<T> policy;

      Policy<T>::Pointer p = 
        policy.allocate(1);
      CFIX_ASSERT(p != NULL);

      policy.deallocate(p, 1);

      p = policy.allocate(100);
      CFIX_ASSERT(p != NULL);

      policy.deallocate(p, 100);
    }

    template<typename T, typename U>
    void comparison()
    {
      Policy<T> policy;
      Policy<U> otherPolicy;
      CFIX_ASSERT(policy == otherPolicy);
      CFIX_ASSERT(!(policy == 10));
    }
  };

public:
  void DynamicAllocatorPolicyTypedTests()
  {
    AllocationPolicyTests<DynamicAllocationPolicy> tests;

    tests.allocDealloc<int>();
    tests.allocDealloc<char>();
    tests.allocDealloc<DummyClass>();
    tests.comparison<DummyClass, DummyClass>();
    tests.comparison<int, char>();
    tests.comparison<DummyClass, int>();
  }
};

CFIXCC_BEGIN_CLASS(TestAllocationPolicy)
  CFIXCC_METHOD(DynamicAllocatorPolicyTypedTests)
CFIXCC_END_CLASS()
