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
    ResourceCollection::Iterator found;
    found = collection.find(path);
    CFIX_ASSERT (found.empty() == false);
    CFIX_ASSERT (found.path()->matches(path));
    CFIX_ASSERT (static_cast<TestResource*>(found.value())->Id() == item.Id());
  }

public:
  void ConstructDestruct()
  {
    StaticPool<10, ResourceCollection::Container> allocator;
    ResourceCollection resources(allocator);

    // CFIX_ASSERT (resources.getCount() == 0);

    CFIX_ASSERT (resources.exists("system.anything") == false);
    CFIX_ASSERT (resources.exists("system") == false);
    CFIX_ASSERT (resources.exists("") == false);
  }

  void AddSimple()
  {
    StaticPool<10, ResourceCollection::Container> allocator;
    ResourceCollection resources(allocator);

    // Add a root
    TestResource testResource0(0);
    CFIX_ASSERT (resources.add("root", &testResource0) == true);

    VerifyFind(resources, "root", testResource0);

    //
    // Add siblings
    //

    TestResource testResource1(1);
    CFIX_ASSERT (resources.add("system.devices.test1", &testResource1) == true);

    TestResource testResource2(2);
    CFIX_ASSERT (resources.add("system.devices.test2", &testResource2) == true);

    VerifyFind(resources, "system.devices.test1", testResource1);
    VerifyFind(resources, "system.devices.test2", testResource2);

    //
    // Add children
    //

    TestResource testResource3(3);
    CFIX_ASSERT (resources.add("system.devices.test1.somethingnew", &testResource3) == true);

    TestResource testResource4(4);
    CFIX_ASSERT (resources.add("system.devices.test2.somethingelsenew", &testResource4) == true);

    VerifyFind(resources, "system.devices.test1.somethingnew", testResource3);
    VerifyFind(resources, "system.devices.test2.somethingelsenew", testResource4);

    //
    // Add parents
    //

    TestResource testResource5(5);
    CFIX_ASSERT (resources.add("system.objects", &testResource5) == true);

    TestResource testResource6(6);
    CFIX_ASSERT (resources.add("diagnostics", &testResource6) == true);

    VerifyFind(resources, "system.objects", testResource5);
    VerifyFind(resources, "diagnostics", testResource6);

    //
    // Keep adding until allocator runs out
    //

    TestResource exhausted(9999);
    CFIX_ASSERT (resources.add("diagnostics.node.7", &exhausted) == true);
    CFIX_ASSERT (resources.add("diagnostics.node.8", &exhausted) == true);
    CFIX_ASSERT (resources.add("diagnostics.node.9", &exhausted) == true);
    CFIX_ASSERT (resources.add("diagnostics.node.10", &exhausted) == false);
    CFIX_ASSERT (resources.add("diagnostics.node.11", &exhausted) == false);
  }

  void AddWithRootReplacement()
  {
    StaticPool<10, ResourceCollection::Container> allocator;
    ResourceCollection resources(allocator);

    int id = 1;

    TestResource testResource1(id++);
    CFIX_ASSERT (resources.add("a.b.c", &testResource1) == true);

    TestResource testResource2(id++);
    CFIX_ASSERT (resources.add("a.b.d", &testResource2) == true);

    TestResource testResource3(id++);
    CFIX_ASSERT (resources.add("a.c.a", &testResource3) == true);

    TestResource testResource4(id++);
    CFIX_ASSERT (resources.add("a.c.b", &testResource4) == true);

    TestResource testResource5(id++);
    CFIX_ASSERT (resources.add("c.a.b", &testResource5) == true);

    TestResource testResource6(id++);
    CFIX_ASSERT (resources.add("a.c.a.1", &testResource6) == true);

    TestResource testResource7(id++);
    CFIX_ASSERT (resources.add("a.c.a.2", &testResource7) == true);

    TestResource testResource8(id++);
    CFIX_ASSERT (resources.add("a.b.c.1", &testResource8) == true);

    TestResource testResource9(id++);
    CFIX_ASSERT (resources.add("a.b.c.2", &testResource9) == true);

    //
    // Insert a node that should become a parent to a.c.a as a sibling to a.b.d.
    // Note: this new node should also become a parent to a.c.b, and a sibling to
    // c.a.b.
    //
    TestResource testResource10(id++);
    CFIX_ASSERT (resources.add("a.c", &testResource10) == true);

    //
    // Verify all nodes are still discoverable
    //

    VerifyFind(resources, "a.b.c", testResource1);
    VerifyFind(resources, "a.b.d", testResource2);
    VerifyFind(resources, "a.c.a", testResource3);
    VerifyFind(resources, "a.c.b", testResource4);
    VerifyFind(resources, "c.a.b", testResource5);
    VerifyFind(resources, "a.c.a.1", testResource6);
    VerifyFind(resources, "a.c.a.2", testResource7);
    VerifyFind(resources, "a.b.c.1", testResource8);
    VerifyFind(resources, "a.b.c.2", testResource9);
    VerifyFind(resources, "a.c", testResource10);

    //
    // Verify expected relationships
    //

    ResourceCollection::Iterator aDotC = resources.find("a.c");
    CFIX_ASSERT (aDotC.empty() == false);
  }

  void Iterate()
  {
  }
};

CFIXCC_BEGIN_CLASS(TestResourceCollection)
  CFIXCC_METHOD(ConstructDestruct)
  CFIXCC_METHOD(AddSimple)
  CFIXCC_METHOD(AddWithRootReplacement)
  CFIXCC_METHOD(Iterate)
CFIXCC_END_CLASS()
