#include <cfixcc.h>
#include <string.h>

#include "ResourceCollection.h"
#include "StaticPool.h"
#include "Resource.h"

class TestResource : public ResourceBase
{
public:
  TestResource(int id) :
    mId(id)
  {
  }
  
  int Id() const { return mId; }
private:
  int mId;
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

    ResourceCollection::ResourceContainer* found = NULL;

    // Add siblings
    TestResource testResource1(1);
    CFIX_ASSERT (resources.add("system.devices.test1", &testResource1) == true);

    TestResource testResource2(2);
    CFIX_ASSERT (resources.add("system.devices.test2", &testResource2) == true);

    found = resources.find("system.devices.test1");
    CFIX_ASSERT (found != NULL);
    CFIX_ASSERT (found->getPath().matches("system.devices.test1"));
    CFIX_ASSERT (static_cast<TestResource*>(found->getValue())->Id() == 1);

    found = resources.find("system.devices.test2");
    CFIX_ASSERT (found != NULL);
    CFIX_ASSERT (found->getPath().matches("system.devices.test2"));
    CFIX_ASSERT (static_cast<TestResource*>(found->getValue())->Id() == 2);

    // Add children
    TestResource testResource3(3);
    CFIX_ASSERT (resources.add("system.devices.test1.somethingnew", &testResource3) == true);

    TestResource testResource4(4);
    CFIX_ASSERT (resources.add("system.devices.test2.somethingelsenew", &testResource4) == true);

    found = resources.find("system.devices.test1.somethingnew");
    CFIX_ASSERT (found != NULL);
    CFIX_ASSERT (found->getPath().matches("system.devices.test1.somethingnew"));
    CFIX_ASSERT (static_cast<TestResource*>(found->getValue())->Id() == 3);

    found = resources.find("system.devices.test2.somethingelsenew");
    CFIX_ASSERT (found != NULL);
    CFIX_ASSERT (found->getPath().matches("system.devices.test2.somethingelsenew"));
    CFIX_ASSERT (static_cast<TestResource*>(found->getValue())->Id() == 4);

    // Add parent
    TestResource testResource5(5);
    CFIX_ASSERT (resources.add("system.objects", &testResource5) == true);

    found = resources.find("system.objects");
    CFIX_ASSERT (found != NULL);
    CFIX_ASSERT (found->getPath().matches("system.objects"));
    CFIX_ASSERT (static_cast<TestResource*>(found->getValue())->Id() == 5);
  }
};

CFIXCC_BEGIN_CLASS(TestResourceCollection)
	CFIXCC_METHOD(ConstructDestruct)
  CFIXCC_METHOD(Add)
CFIXCC_END_CLASS()
