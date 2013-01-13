#include <cfixcc.h>
#include <string.h>

#include "StaticPool.h"

class TestStaticPool : public cfixcc::TestFixture
{
private:
  
  class TestClass
  {
  public:
    TestClass() :
      mSomeData(0)
    {
    }

  private:
    int mSomeData;
  };

public:
  void AllocateDeallocate()
  {
    StaticPool<10, TestClass> pool;

    for (int times = 0; times < pool.getSize(); times++)
    {
      TestClass* cls = pool.allocate();
      CFIXCC_ASSERT (cls != NULL);
      CFIXCC_ASSERT (pool.getAllocated() == times + 1);
    }

    // Full, no more allocations
    CFIXCC_ASSERT (pool.getAllocated() == pool.getSize());
    CFIXCC_ASSERT (pool.allocate() == NULL);
  }
};

CFIXCC_BEGIN_CLASS(TestStaticPool)
	CFIXCC_METHOD(AllocateDeallocate)
CFIXCC_END_CLASS()