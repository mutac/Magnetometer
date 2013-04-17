#include <cfixcc.h>
#include <string.h>
#include <iostream>
#include <set>

#include "PathCollection.h"

class TestPathCollection : public cfixcc::TestFixture
{
private:
  class TestResource
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

  typedef PathCollection<TestResource> TestCollection;

  void VerifyFind(TestCollection& collection, const char* path, 
    TestResource& item)
  {
    TestCollection found = collection.find(path);
    CFIX_ASSERT (found.empty() == false);

    TestCollection::Iterator foundItem = found.begin();
    CFIX_ASSERT (foundItem.path()->matches(path));
    CFIX_ASSERT (static_cast<TestResource*>(foundItem.value())->Id() == item.Id());
  }

  void VerifyIteration(const TestCollection& collection, 
    const std::set<PathName>& expected)
  {
    std::set<PathName> foundPaths;

    unsigned int expectedCount = 0; 
    for(TestCollection::Iterator it = collection.begin();
        it != collection.end() && expectedCount < expected.size();
        expectedCount++, it++)
    {
      // Only appear once?
      CFIX_ASSERT (foundPaths.find(*it.path()) == foundPaths.end());
      foundPaths.insert(*it.path());
    }

    CFIX_ASSERT (expectedCount == expected.size());
    CFIX_ASSERT (expected == foundPaths);
  }

public:
  void ConstructDestruct()
  {
    //StaticPool<10, TestCollection::PathTree> allocator;
    TestCollection tree;

    CFIX_ASSERT (tree.empty());

    CFIX_ASSERT (tree.exists("anything.at.all") == false);
    CFIX_ASSERT (tree.exists("nothing") == false);
    CFIX_ASSERT (tree.exists("*") == false);
    CFIX_ASSERT (tree.exists("") == false);
  }

  void AddSimple()
  {
    TestCollection tree;

    // Add a root
    TestResource testResource0;
    CFIX_ASSERT (tree.add("root", &testResource0) == true);

    VerifyFind(tree, "root", testResource0);

    //
    // Add siblings
    //

    TestResource testResource1;
    CFIX_ASSERT (tree.add("system.devices.test1", &testResource1) == true);

    TestResource testResource2;
    CFIX_ASSERT (tree.add("system.devices.test2", &testResource2) == true);

    VerifyFind(tree, "system.devices.test1", testResource1);
    VerifyFind(tree, "system.devices.test2", testResource2);

    //
    // Add children
    //

    TestResource testResource3;
    CFIX_ASSERT (tree.add("system.devices.test1.somethingnew", &testResource3) == true);

    TestResource testResource4;
    CFIX_ASSERT (tree.add("system.devices.test2.somethingelsenew", &testResource4) == true);

    VerifyFind(tree, "system.devices.test1.somethingnew", testResource3);
    VerifyFind(tree, "system.devices.test2.somethingelsenew", testResource4);

    //
    // Add parents
    //

    TestResource testResource5;
    CFIX_ASSERT (tree.add("system.objects", &testResource5) == true);

    TestResource testResource6;
    CFIX_ASSERT (tree.add("diagnostics", &testResource6) == true);

    VerifyFind(tree, "system.objects", testResource5);
    VerifyFind(tree, "diagnostics", testResource6);
  }

  void AddWithRootReplacement()
  {
    TestCollection tree;

    TestResource testResource1;
    CFIX_ASSERT (tree.add("a.b.c", &testResource1) == true);

    TestResource testResource2;
    CFIX_ASSERT (tree.add("a.b.d", &testResource2) == true);

    TestResource testResource3;
    CFIX_ASSERT (tree.add("a.c.a", &testResource3) == true);

    TestResource testResource4;
    CFIX_ASSERT (tree.add("a.c.b", &testResource4) == true);

    TestResource testResource5;
    CFIX_ASSERT (tree.add("c.a.b", &testResource5) == true);

    TestResource testResource6;
    CFIX_ASSERT (tree.add("a.c.a.1", &testResource6) == true);

    TestResource testResource7;
    CFIX_ASSERT (tree.add("a.c.a.2", &testResource7) == true);

    TestResource testResource8;
    CFIX_ASSERT (tree.add("a.b.c.1", &testResource8) == true);

    TestResource testResource9;
    CFIX_ASSERT (tree.add("a.b.c.2", &testResource9) == true);

    //
    // Insert a node that should become a parent to a.c.a as a sibling to a.b.d.
    // Note: this new node should also become a parent to a.c.b, and a sibling to
    // c.a.b.
    //
    TestResource testResource10;
    CFIX_ASSERT (tree.add("a.c", &testResource10) == true);

    //
    // Verify all nodes are still discoverable
    //

    VerifyFind(tree, "a.b.c", testResource1);
    VerifyFind(tree, "a.b.d", testResource2);
    VerifyFind(tree, "a.c.a", testResource3);
    VerifyFind(tree, "a.c.b", testResource4);
    VerifyFind(tree, "c.a.b", testResource5);
    VerifyFind(tree, "a.c.a.1", testResource6);
    VerifyFind(tree, "a.c.a.2", testResource7);
    VerifyFind(tree, "a.b.c.1", testResource8);
    VerifyFind(tree, "a.b.c.2", testResource9);
    VerifyFind(tree, "a.c", testResource10);

    //
    // Verify expected relationships
    //

    TestCollection aDotC = tree.find("a.c");
    CFIX_ASSERT (aDotC.empty() == false);
  }

  void Iterate()
  {
    TestCollection metal;
    TestCollection::Iterator types;

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
    const TestCollection empty = metal.find("screamo");
    CFIX_ASSERT (empty.begin() == empty.end());

    // Single element
    const TestCollection grunge = metal.find("earlymetal.grunge");

    std::set<PathName> expectedGrunge;
    expectedGrunge.insert("earlymetal.grunge");

    VerifyIteration(grunge, expectedGrunge);

    // Multiple siblings
    const TestCollection shockRock = metal.find("earlymetal.shockrock.*");

    std::set<PathName> expectedShockRock;
    expectedShockRock.insert("earlymetal.shockrock.glamrock");
    expectedShockRock.insert("earlymetal.shockrock.industrialmetal");
    expectedShockRock.insert("earlymetal.shockrock.industrialmetal.numetal");

    VerifyIteration(shockRock, expectedShockRock);

    // Everything
    const TestCollection earlyMetal = metal.find("earlymetal.*");

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

int TestPathCollection::TestResource::sNextId = 0;

CFIXCC_BEGIN_CLASS(TestPathCollection)
  CFIXCC_METHOD(ConstructDestruct)
  CFIXCC_METHOD(AddSimple)
  CFIXCC_METHOD(AddWithRootReplacement)
  CFIXCC_METHOD(Iterate)
CFIXCC_END_CLASS()
