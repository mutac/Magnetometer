#include <cfixcc.h>
#include "Allocator.h"

class TestAllocator : public cfixcc::TestFixture
{
public:
  void Simple()
  {
  }
};

CFIXCC_BEGIN_CLASS(TestAllocator)
  CFIXCC_METHOD(Simple)
CFIXCC_END_CLASS()