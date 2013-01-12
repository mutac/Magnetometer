#include <cfixcc.h>
#include <string.h>

#include "StaticResourceCollection.h"
#include "Resource.h"

class TestResource1 : public ResourceBase
{
};

class TestStaticResourceCollection : public cfixcc::TestFixture
{
private:
public:
  void ConstructDestruct()
  {
    StaticResourceCollection<10> resources;
    CFIX_ASSERT (resources.getCount() == 0);

    CFIX_ASSERT (resources.exists("system.anything") == false);
    CFIX_ASSERT (resources.exists("system") == false);
    CFIX_ASSERT (resources.exists("") == false);
  }

  void Add()
  {
    StaticResourceCollection<10> resources;
    CFIX_ASSERT (resources.getCount() == 0);

    TestResource1 testResource1;
    CFIX_ASSERT (resources.add("system.devices.test", &testResource1) == true);
  }
};

CFIXCC_BEGIN_CLASS(TestStaticResourceCollection)
	CFIXCC_METHOD(ConstructDestruct)
CFIXCC_END_CLASS()
