#include <cfixcc.h>
#include "ObjectTraits.h"

class TestObjectTraits : public cfixcc::TestFixture
{
public:
  void Simple()
  {
    ObjectTraits<int> intTraits;

    int here = 1234;
    int* where = intTraits.addressOf(here);
    CFIX_ASSERT(where == &here);

    int const& hereConst = here;
    int const* whereConst = intTraits.addressOf(hereConst);
    CFIX_ASSERT(whereConst == &hereConst && whereConst == &here);

    int* newInt = NULL;
    intTraits.construct(newInt);
    CFIX_ASSERT(newInt != NULL);
  }
};

CFIXCC_BEGIN_CLASS(TestObjectTraits)
  CFIXCC_METHOD(Simple)
CFIXCC_END_CLASS()
