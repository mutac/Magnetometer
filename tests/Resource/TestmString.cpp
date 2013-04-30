#include <cfixcc.h>
#include "mString.h"

class TestString : public cfixcc::TestFixture
{
public:
  void Simple()
  {
    mString empty;
    CFIX_ASSERT(empty.empty());

    mString unowned((const char*)"hello");
    CFIX_ASSERT(unowned == "hello");
    CFIX_ASSERT(unowned.isOwned() == false);

    mString owned((char*)"hello");
    CFIX_ASSERT(owned == "hello");
    CFIX_ASSERT(owned == unowned);
    CFIX_ASSERT(owned.isOwned());

    mString unownedCopy(unowned);
    CFIX_ASSERT(unownedCopy == unowned);
    CFIX_ASSERT(unownedCopy.isOwned() == false);

    mString ownedCopy(owned);
    CFIX_ASSERT(ownedCopy == owned);
    CFIX_ASSERT(ownedCopy.isOwned());

    mString outer = "omg";
    {
      mString scoped = "some const string";
      outer = scoped;
    }
    CFIX_ASSERT(outer == "some const string");

    outer = "omg again";
    {
      mString scoped = const_cast<char*>("an owned string to copy");
      outer = scoped;
    }
    CFIX_ASSERT(outer == "an owned string to copy");
  }

  void Numeric()
  {
    mString fortyTwo(42);
    CFIX_ASSERT(fortyTwo == "42");

    mString piFloat(3.14159f);
    CFIX_ASSERT(piFloat == "3.14159");

    mString piDouble(3.1415926);
    CFIX_ASSERT(piFloat == "3.14159");
  }

  void Append()
  {
    bool succeeded = false;

    mString startEmpty;

    succeeded = startEmpty.append("one ");
    CFIX_ASSERT(succeeded);
    CFIX_ASSERT(startEmpty == "one ");

    succeeded = startEmpty.append("two ");
    CFIX_ASSERT(succeeded);
    CFIX_ASSERT(startEmpty == "one two ");

    succeeded = startEmpty.append("three ");
    CFIX_ASSERT(succeeded);
    CFIX_ASSERT(startEmpty == "one two three ");
  }
};

CFIXCC_BEGIN_CLASS(TestString)
  CFIXCC_METHOD(Simple)
  CFIXCC_METHOD(Numeric)
  CFIXCC_METHOD(Append)
CFIXCC_END_CLASS()