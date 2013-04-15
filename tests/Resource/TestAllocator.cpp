#include <cfixcc.h>
#include "Allocator.h"

class TestAllocator : public cfixcc::TestFixture
{
private:
  class ClassObserver
  {
  public:
    ClassObserver() :
      mConstructorCalled(false),
      mDestructorCalled(false)
    {
    }

    void onConstruct()
    {
      mConstructorCalled = true;
    }

    bool constructorCalled() const
    {
      return mConstructorCalled;
    }

    void onDestruct()
    {
      mDestructorCalled = true;
    }

    bool destructorCalled() const
    {
      return mDestructorCalled;
    }

    void reset() 
    {
      mConstructorCalled = false;
      mDestructorCalled = false;
    }

  private:
    bool mConstructorCalled;
    bool mDestructorCalled;
  };

  class ObservableClass
  {
  public:
    ObservableClass(ClassObserver& observer)
      : mObserver(observer)
    {
      mObserver.onConstruct();
    }

    ~ObservableClass()
    {
      mObserver.onDestruct();
    }

  private:
    ClassObserver& mObserver;
  };

  class AllocatorTests
  {
  public:
    template<class AllocatorType>
    void allocateConstructDestructDeallocate()
    {
      ClassObserver observer;
      AllocatorType::rebind<ObservableClass>::other allocator;

      ObservableClass* p = allocator.allocate(1);
      CFIX_ASSERT(p != NULL);

      allocator.construct(p, ObservableClass(observer));
      CFIX_ASSERT(observer.constructorCalled());

      allocator.destroy(p);
      CFIX_ASSERT(observer.destructorCalled());

      allocator.deallocate(p, 1);
    }
  };

public:
  void AllocateConstructDestructDeallocate()
  {
    AllocatorTests tests;

    tests.allocateConstructDestructDeallocate<Allocator<int> >();
  }
};

CFIXCC_BEGIN_CLASS(TestAllocator)
  CFIXCC_METHOD(AllocateConstructDestructDeallocate)
CFIXCC_END_CLASS()