#include <cfixcc.h>
#include <string.h>

#include "StaticResourceCollection.h"
#include "StaticPool.h"
#include "Resource.h"

class TestResource1 : public ResourceBase
{
};

class TestStaticResourceCollection : public cfixcc::TestFixture
{
private:
public:
  void ConstructDestruct()
  {
    StaticPool<10, ResourceContainer> allocator;
    StaticResourceCollection resources(allocator);

    // CFIX_ASSERT (resources.getCount() == 0);

    CFIX_ASSERT (resources.exists("system.anything") == false);
    CFIX_ASSERT (resources.exists("system") == false);
    CFIX_ASSERT (resources.exists("") == false);
  }

  void Add()
  {
    StaticPool<10, ResourceContainer> allocator;
    StaticResourceCollection resources(allocator);

    TestResource1 testResource1;
    CFIX_ASSERT (resources.add("system.devices.test", &testResource1) == true);
  }
};

CFIXCC_BEGIN_CLASS(TestStaticResourceCollection)
	CFIXCC_METHOD(ConstructDestruct)
  CFIXCC_METHOD(Add)
CFIXCC_END_CLASS()
