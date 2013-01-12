#include <cfixcc.h>
#include <string.h>

#include "StaticPool.h"

class TestStaticPool : public cfixcc::TestFixture
{
private:
  class TestClass
  {
  private:
    int mSomeData;
  };
public:
  void AllocateDeallocate()
  {
    StaticPool<10, TestClass> pool10;

    for (int times = 0; times < 10; times++)
    {
      TestClass* cls = pool10.allocate();
      CFIXCC_ASSERT (cls != NULL);
    }

    // Full, no more allocations
    CFIXCC_ASSERT (pool10.allocate() == NULL);
  }
};

CFIXCC_BEGIN_CLASS(TestStaticPool)
	CFIXCC_METHOD(AllocateDeallocate)
CFIXCC_END_CLASS()