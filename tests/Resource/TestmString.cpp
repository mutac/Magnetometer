#include <cfixcc.h>
#include "mString.h"

class TestString : public cfixcc::TestFixture
{
public:
  void Simple()
  {
  }
};

CFIXCC_BEGIN_CLASS(TestString)
  CFIXCC_METHOD(Simple)
CFIXCC_END_CLASS()