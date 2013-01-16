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
  void VerifyFind(ResourceCollection& collection, const char* path, TestResource& item)
  {
    ResourceCollection::ResourceContainer* found;
    found = collection.find(path);
    CFIX_ASSERT (found != NULL);
    CFIX_ASSERT (found->getPath().matches(path));
    CFIX_ASSERT (static_cast<TestResource*>(found->getValue())->Id() == item.Id());
  }

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

    // Add a root
    TestResource testResource0(0);
    CFIX_ASSERT (resources.add("root", &testResource0) == true);

    VerifyFind(resources, "root", testResource0);

    // Add siblings
    TestResource testResource1(1);
    CFIX_ASSERT (resources.add("system.devices.test1", &testResource1) == true);

    TestResource testResource2(2);
    CFIX_ASSERT (resources.add("system.devices.test2", &testResource2) == true);

    VerifyFind(resources, "system.devices.test1", testResource1);
    VerifyFind(resources, "system.devices.test2", testResource2);

    // Add children
    TestResource testResource3(3);
    CFIX_ASSERT (resources.add("system.devices.test1.somethingnew", &testResource3) == true);

    TestResource testResource4(4);
    CFIX_ASSERT (resources.add("system.devices.test2.somethingelsenew", &testResource4) == true);

    VerifyFind(resources, "system.devices.test1.somethingnew", testResource3);
    VerifyFind(resources, "system.devices.test2.somethingelsenew", testResource4);

    // Add parent
    TestResource testResource5(5);
    CFIX_ASSERT (resources.add("system.objects", &testResource5) == true);

    TestResource testResource6(6);
    CFIX_ASSERT (resources.add("diagnostics", &testResource6) == true);

    VerifyFind(resources, "system.objects", testResource5);
    VerifyFind(resources, "diagnostics", testResource6);
  }
};

CFIXCC_BEGIN_CLASS(TestResourceCollection)
	CFIXCC_METHOD(ConstructDestruct)
  CFIXCC_METHOD(Add)
CFIXCC_END_CLASS()
