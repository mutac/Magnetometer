#include <cfixcc.h>
#include "mString.h"

class TestString : public cfixcc::TestFixture
{
public:
  void Referencing()
  {
    mString empty;
    CFIX_ASSERT(empty.empty());

    mString otherEmpty("");
    CFIX_ASSERT(otherEmpty.empty());

    mString s1((const char*)"hello");
    CFIX_ASSERT(s1 == "hello");

    mString s2((char*)"hello");
    CFIX_ASSERT(s2 == "hello");
    CFIX_ASSERT(s2 == s1);

    mString s3(s2);
    CFIX_ASSERT(s3 == s2);

    mString s4(s1);
    CFIX_ASSERT(s4 == s1);

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
    mString fortyTwo = to_string(42);
    CFIX_ASSERT(fortyTwo == "42");

    mString fortyTwoLong = to_string((long)42);
    CFIX_ASSERT(fortyTwoLong == "42");

    //mString piFloat(3.14159f);
    //CFIX_ASSERT(piFloat == "3.14159");

    //mString piDouble(3.1415926);
    //CFIX_ASSERT(piFloat == "3.14159");
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

  void Find()
  {
    bool succeeded = false;
    const char* found = NULL;

    mString aString("This is a long string");

    found = aString.find("is");
    CFIX_ASSERT(found != NULL);
    CFIX_ASSERT(strcmp(found, "is a long string"));

    found = aString.find("nothing");
    CFIX_ASSERT(found == NULL);

    succeeded = aString.beginsWith("This");
    CFIX_ASSERT(succeeded);
  }

  void Iterate()
  {
    mString aString("This is a space separated string.");

    mString::Iterator it = aString.split(" ");
    CFIX_ASSERT(*it == "This");
    ++it;
    CFIX_ASSERT(*it == "is");
    ++it;
    CFIX_ASSERT(*it == "a");
    ++it;
    CFIX_ASSERT(*it == "space");
    ++it;
    CFIX_ASSERT(*it == "separated");
    ++it;
    CFIX_ASSERT(*it == "string.");
    ++it;
    CFIX_ASSERT(it == aString.end());

    mString another("abcd");
    mString::Iterator it2 = another.begin();
    CFIX_ASSERT(*it2 == "a");
    ++it2;
    CFIX_ASSERT(*it2 == "b");
    ++it2;
    CFIX_ASSERT(*it2 == "c");
    ++it2;
    CFIX_ASSERT(*it2 == "d");
    ++it2;
    CFIX_ASSERT(it2 == another.end());
  }

  void Mutate()
  {
    mString m1("woohoo");
    const mString m2(m1);

    //
    // const operations should maintain the common reference
    // between the two string object.
    //
    CFIX_ASSERT(m1.c_str() == m2.c_str());
    m1.beginsWith("w");
    CFIX_ASSERT(m1.c_str() == m2.c_str());
    m2[0] == 'w';
    CFIX_ASSERT(m1.c_str() == m2.c_str());

    //
    // Non-const operations should cause one to create
    // a new, unreferenced, string buffer.
    //
    m1[0] = 'a';
    CFIXCC_ASSERT(m1 == "aoohoo");
    CFIX_ASSERT(m1.c_str() != m2.c_str());
    CFIXCC_ASSERT(m2 == "woohoo");

    mString m3(m2);
    CFIX_ASSERT(m3.c_str() == m2.c_str());
    m3.append("doo");
    CFIXCC_ASSERT(m3 == "woohoodoo");
    CFIX_ASSERT(m3.c_str() != m2.c_str());
    CFIXCC_ASSERT(m2 == "woohoo");
  }
};

CFIXCC_BEGIN_CLASS(TestString)
  CFIXCC_METHOD(Referencing)
  CFIXCC_METHOD(Numeric)
  CFIXCC_METHOD(Append)
  CFIXCC_METHOD(Find)
  CFIXCC_METHOD(Iterate)
  CFIXCC_METHOD(Mutate)
CFIXCC_END_CLASS()