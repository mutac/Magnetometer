#include <cfixcc.h>
#include <string.h>
#include <iostream>
#include <set>

#include "ResourceCollection.h"
#include "StaticPool.h"
#include "Resource.h"

class TestResourceCollection : public cfixcc::TestFixture
{
private:
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

  void VerifyFind(ResourceCollection& collection, const char* path, 
    TestResource& item)
  {
    ResourceCollection found = collection.find(path);
    CFIX_ASSERT (found.empty() == false);

    ResourceCollection::Iterator foundItem = found.begin();
    CFIX_ASSERT (foundItem.path()->matches(path));
    CFIX_ASSERT (static_cast<TestResource*>(foundItem.value())->Id() == item.Id());
  }

  void VerifyIteration(const ResourceCollection& collection, 
    const std::set<PathName>& expected)
  {
    std::set<PathName> foundPaths;

    for(ResourceCollection::Iterator it = collection.begin(); 
        it != collection.end(); 
        ++it)
    {
      CFIX_ASSERT (foundPaths.find(*it.path()) == foundPaths.end());
      foundPaths.insert(*it.path());
    }

    CFIX_ASSERT (expected == foundPaths);
  }

public:
  void ConstructDestruct()
  {
    StaticPool<10, ResourceCollection::PathTree> allocator;
    ResourceCollection metal(allocator);

    // CFIX_ASSERT (resources.getCount() == 0);

    CFIX_ASSERT (metal.exists("anything.at.all") == false);
    CFIX_ASSERT (metal.exists("nothing") == false);
    CFIX_ASSERT (metal.exists("*") == false);
    CFIX_ASSERT (metal.exists("") == false);
  }

  void AddSimple()
  {
    StaticPool<10, ResourceCollection::PathTree> allocator;
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
    StaticPool<10, ResourceCollection::PathTree> allocator;
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
    StaticPool<10, ResourceCollection::PathTree> allocator;
    ResourceCollection metal(allocator);
    ResourceCollection::Iterator types;

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

    // Failed search
    const ResourceCollection empty = metal.find("screamo");
    CFIX_ASSERT (empty.begin() == empty.end());

    // Single element
    const ResourceCollection grunge = metal.find("earlymetal.grunge");

    std::set<PathName> expectedGrunge;
    expectedGrunge.insert("earlymetal.grunge");

    VerifyIteration(grunge, expectedGrunge);

    // Multiple siblings
    const ResourceCollection shockRock = metal.find("earlymetal.shockrock.*");

    std::set<PathName> expectedShockRock;
    expectedShockRock.insert("earlymetal.shockrock.glamrock");
    expectedShockRock.insert("earlymetal.shockrock.industrialmetal");
    expectedShockRock.insert("earlymetal.shockrock.industrialmetal.numetal");

    VerifyIteration(shockRock, expectedShockRock);

    // Everything
    const ResourceCollection earlyMetal = metal.find("earlymetal.*");

    std::set<PathName> expectedEarlyMetal;
    expectedEarlyMetal.insert("earlymetal.grunge");
    expectedEarlyMetal.insert("earlymetal.powermetal");
    expectedEarlyMetal.insert("earlymetal.shockrock");
    expectedEarlyMetal.insert("earlymetal.powermetal.britishheavymetal");
    expectedEarlyMetal.insert("earlymetal.shockrock.glamrock");
    expectedEarlyMetal.insert("earlymetal.shockrock.industrialmetal");
    expectedEarlyMetal.insert("earlymetal.shockrock.industrialmetal.numetal");

    VerifyIteration(earlyMetal, expectedEarlyMetal);
  }
};

int TestResourceCollection::TestResource::sNextId = 0;

CFIXCC_BEGIN_CLASS(TestResourceCollection)
  CFIXCC_METHOD(ConstructDestruct)
  CFIXCC_METHOD(AddSimple)
  CFIXCC_METHOD(AddWithRootReplacement)
  CFIXCC_METHOD(Iterate)
CFIXCC_END_CLASS()
