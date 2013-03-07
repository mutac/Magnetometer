#include <cfixcc.h>
#include "ObjectTraits.h"

class TestObjectTraits : public cfixcc::TestFixture
{
private:
  class TestObject
  {
  public:
    TestObject() :
      constructed(true),
      destructed(false)
    {
      constructionCount++;
    }

    ~TestObject()
    {
      destructed = true;
    }

    bool constructed;
    bool destructed;
    static int constructionCount;
  };

public:
  void IntTraits()
  {
    ObjectTraits<int> intTraits;

    int here = 1234;
    int* where = intTraits.addressOf(here);
    CFIX_ASSERT(where == &here);

    int const& hereConst = here;
    int const* whereConst = intTraits.addressOf(hereConst);
    CFIX_ASSERT(whereConst == &hereConst && whereConst == &here);

    int newInt = 10;
    int anInt = 100;
    intTraits.construct(&newInt, anInt);
    CFIX_ASSERT(anInt == 100);
  }

  void ClassTraits()
  {
    ObjectTraits<TestObject> testObjectTraits;

    TestObject here;
    TestObject* where = testObjectTraits.addressOf(here);
    CFIX_ASSERT(where == &here);

    TestObject const& hereConst = here;
    TestObject const* whereConst = testObjectTraits.addressOf(hereConst);
    CFIX_ASSERT(whereConst == &hereConst && whereConst == &here);

    TestObject newObject;
    CFIX_ASSERT(newObject.constructionCount == 2);

    TestObject anObject;
    CFIX_ASSERT(anObject.constructionCount == 3);

    testObjectTraits.construct(&newObject, anObject);
    CFIX_ASSERT(newObject.constructionCount == 3);

    testObjectTraits.destroy(&newObject);
    CFIX_ASSERT(newObject.destructed == true);
  }
};

int TestObjectTraits::TestObject::constructionCount;

CFIXCC_BEGIN_CLASS(TestObjectTraits)
  CFIXCC_METHOD(IntTraits)
  CFIXCC_METHOD(ClassTraits)
CFIXCC_END_CLASS()
