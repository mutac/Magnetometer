#include <cfixcc.h>
#include <string.h>

#include "ResourceCollection.h"
#include "StaticPool.h"
#include "Resource.h"

class TestResource : public ResourceBase
{
public:
  TestResource() :
    mId(sNextId)
  {
    sNextId++;
  }

  TestResource(const TestResource& rhs) :
    mId(rhs.mId)
  {
  }

  int Id() const { return mId; }

private:
  int mId;
  static int sNextId;
};

int TestResource::sNextId = 0;

class TestResourceCollection : public cfixcc::TestFixture
{
private:
  void VerifyFind(ResourceCollection& collection, const char* path, TestResource& item)
  {
    ResourceCollection found = collection.find(path);
    CFIX_ASSERT (found.empty() == false);

    ResourceCollection::Iterator foundItem = found.begin();
    CFIX_ASSERT (foundItem.path()->matches(path));
    CFIX_ASSERT (static_cast<TestResource*>(foundItem.value())->Id() == item.Id());
  }

public:
  void ConstructDestruct()
  {
    StaticPool<10, ResourceCollection::Container> allocator;
    ResourceCollection metal(allocator);

    // CFIX_ASSERT (resources.getCount() == 0);

    CFIX_ASSERT (metal.exists("system.anything") == false);
    CFIX_ASSERT (metal.exists("system") == false);
    CFIX_ASSERT (metal.exists("") == false);
  }

  void AddSimple()
  {
    StaticPool<10, ResourceCollection::Container> allocator;
    ResourceCollection metal(allocator);

    // Add a root
    TestResource testResource0;
    CFIX_ASSERT (metal.add("root", &testResource0) == true);

    VerifyFind(metal, "root", testResource0);

    //
    // Add siblings
    //

    TestResource testResource1;
    CFIX_ASSERT (metal.add("system.devices.test1", &testResource1) == true);

    TestResource testResource2;
    CFIX_ASSERT (metal.add("system.devices.test2", &testResource2) == true);

    VerifyFind(metal, "system.devices.test1", testResource1);
    VerifyFind(metal, "system.devices.test2", testResource2);

    //
    // Add children
    //

    TestResource testResource3;
    CFIX_ASSERT (metal.add("system.devices.test1.somethingnew", &testResource3) == true);

    TestResource testResource4;
    CFIX_ASSERT (metal.add("system.devices.test2.somethingelsenew", &testResource4) == true);

    VerifyFind(metal, "system.devices.test1.somethingnew", testResource3);
    VerifyFind(metal, "system.devices.test2.somethingelsenew", testResource4);

    //
    // Add parents
    //

    TestResource testResource5;
    CFIX_ASSERT (metal.add("system.objects", &testResource5) == true);

    TestResource testResource6;
    CFIX_ASSERT (metal.add("diagnostics", &testResource6) == true);

    VerifyFind(metal, "system.objects", testResource5);
    VerifyFind(metal, "diagnostics", testResource6);

    //
    // Keep adding until allocator runs out
    //

    TestResource exhausted;
    CFIX_ASSERT (metal.add("diagnostics.node.7", &exhausted) == true);
    CFIX_ASSERT (metal.add("diagnostics.node.8", &exhausted) == true);
    CFIX_ASSERT (metal.add("diagnostics.node.9", &exhausted) == true);
    CFIX_ASSERT (metal.add("diagnostics.node.10", &exhausted) == false);
    CFIX_ASSERT (metal.add("diagnostics.node.11", &exhausted) == false);
  }

  void AddWithRootReplacement()
  {
    StaticPool<10, ResourceCollection::Container> allocator;
    ResourceCollection metal(allocator);

    TestResource testResource1;
    CFIX_ASSERT (metal.add("a.b.c", &testResource1) == true);

    TestResource testResource2;
    CFIX_ASSERT (metal.add("a.b.d", &testResource2) == true);

    TestResource testResource3;
    CFIX_ASSERT (metal.add("a.c.a", &testResource3) == true);

    TestResource testResource4;
    CFIX_ASSERT (metal.add("a.c.b", &testResource4) == true);

    TestResource testResource5;
    CFIX_ASSERT (metal.add("c.a.b", &testResource5) == true);

    TestResource testResource6;
    CFIX_ASSERT (metal.add("a.c.a.1", &testResource6) == true);

    TestResource testResource7;
    CFIX_ASSERT (metal.add("a.c.a.2", &testResource7) == true);

    TestResource testResource8;
    CFIX_ASSERT (metal.add("a.b.c.1", &testResource8) == true);

    TestResource testResource9;
    CFIX_ASSERT (metal.add("a.b.c.2", &testResource9) == true);

    //
    // Insert a node that should become a parent to a.c.a as a sibling to a.b.d.
    // Note: this new node should also become a parent to a.c.b, and a sibling to
    // c.a.b.
    //
    TestResource testResource10;
    CFIX_ASSERT (metal.add("a.c", &testResource10) == true);

    //
    // Verify all nodes are still discoverable
    //

    VerifyFind(metal, "a.b.c", testResource1);
    VerifyFind(metal, "a.b.d", testResource2);
    VerifyFind(metal, "a.c.a", testResource3);
    VerifyFind(metal, "a.c.b", testResource4);
    VerifyFind(metal, "c.a.b", testResource5);
    VerifyFind(metal, "a.c.a.1", testResource6);
    VerifyFind(metal, "a.c.a.2", testResource7);
    VerifyFind(metal, "a.b.c.1", testResource8);
    VerifyFind(metal, "a.b.c.2", testResource9);
    VerifyFind(metal, "a.c", testResource10);

    //
    // Verify expected relationships
    //

    ResourceCollection aDotC = metal.find("a.c");
    CFIX_ASSERT (aDotC.empty() == false);
  }

  void Iterate()
  {
    StaticPool<10, ResourceCollection::Container> allocator;
    ResourceCollection metal(allocator);
    ResourceCollection::Iterator it;

    TestResource res1;
    metal.add("earlymetal", &res1);

    TestResource res2;
    metal.add("earlymetal.grunge", &res2);

    TestResource res3;
    metal.add("earlymetal.powermetal", &res3);

    TestResource res4;
    metal.add("earlymetal.shockrock", &res4);

    TestResource res5;
    metal.add("earlymetal.powermetal.britishheavymetal", &res5);

    TestResource res6;
    metal.add("earlymetal.shockrock.glamrock", &res6);

    TestResource res7;
    metal.add("earlymetal.shockrock.industrialmetal", &res7);

    TestResource res8;
    metal.add("earlymetal.shockrock.industrialmetal.numetal", &res8);

    // Single element
    ResourceCollection grunge = metal.find("earlymetal.grunge");
    it = grunge.begin();
    CFIX_ASSERT (it.path()->matches("earlymetal.grunge"));
    it++;
    CFIX_ASSERT (it == grunge.end());

    // TODO: Add wildcard and non-wildcard searches.  These will influence
    // the iterators traversal.
  }
};

CFIXCC_BEGIN_CLASS(TestResourceCollection)
  CFIXCC_METHOD(ConstructDestruct)
  CFIXCC_METHOD(AddSimple)
  CFIXCC_METHOD(AddWithRootReplacement)
  CFIXCC_METHOD(Iterate)
CFIXCC_END_CLASS()
