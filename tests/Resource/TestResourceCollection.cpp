#include <cfixcc.h>
#include <string.h>

#include "ResourceCollection.h"
#include "StaticPool.h"
#include "Resource.h"

class TestResource : public ResourceBase
{
};

class TestResourceCollection : public cfixcc::TestFixture
{
private:
public:
  void ConstructDestruct()
  {
    StaticPool<10, ResourceCollection::ResourceContainer> allocator;
    ResourceCollection resources(allocator);

    // CFIX_ASSERT (resources.getCount() == 0);

    CFIX_ASSERT (resources.exists("system.anything") == false);
    CFIX_ASSERT (resources.exists("system") == false);
    CFIX_ASSERT (resources.exists("") == false);
  }

  void Add()
  {
    StaticPool<10, ResourceCollection::ResourceContainer> allocator;
    ResourceCollection resources(allocator);

    // Add siblings
    TestResource testResource1;
    CFIX_ASSERT (resources.add("system.devices.test", &testResource1) == true);

    TestResource testResource2;
    CFIX_ASSERT (resources.add("system.devices.test2", &testResource2) == true);

    // Add children
    TestResource testResource3;
    CFIX_ASSERT (resources.add("system.devices.test2.somethingnew", &testResource3) == true);

    TestResource testResource4;
    CFIX_ASSERT (resources.add("system.devices.test2.somethingelsenew", &testResource4) == true);

    // Add parent
    TestResource testResource5;
    CFIX_ASSERT (resources.add("system.objects", &testResource5) == true);
  }
};

CFIXCC_BEGIN_CLASS(TestResourceCollection)
	CFIXCC_METHOD(ConstructDestruct)
  CFIXCC_METHOD(Add)
CFIXCC_END_CLASS()
