#include <cfixcc.h>
#include "Allocator.h"

class TestAllocator : public cfixcc::TestFixture
{
private:
  //template<template<typename T, typename P, typename OT> class AllocatorType>
  //class AllocatorTests
  //{
  //public:
  //  template<typename T>
  //  void allocate()
  //  {
  //    AllocatorType<T, P, OT> allocator;
  //  }
  //}

  class DummyClass
  {
  public:
    DummyClass()
    {
    }
  private:
    char mBigSize[1000];
  };

public:
  void Simple()
  {
    Allocator<DummyClass> allocator;

    DummyClass* p = allocator.allocate(1);
    allocator.construct(p, DummyClass());
    allocator.destroy(p);
    allocator.deallocate(p, 1);
  }
};

CFIXCC_BEGIN_CLASS(TestAllocator)
  CFIXCC_METHOD(Simple)
CFIXCC_END_CLASS()