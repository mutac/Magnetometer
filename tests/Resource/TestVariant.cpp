#include <cfixcc.h>
#include "Variant.h"

class TestVariant : public cfixcc::TestFixture
{
public:
  void SimpleTypes()
  {
    Variant v(10);

    v.set<float>(100.0f);
    float f = v.get<float>();
    CFIX_ASSERT (f == 100.0f);
  }
};

CFIXCC_BEGIN_CLASS(TestVariant)
  CFIXCC_METHOD(SimpleTypes)
CFIXCC_END_CLASS()