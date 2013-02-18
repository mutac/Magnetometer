
#include <cfixcc.h>
#include "SharedCount.h"

class TestSharedCount : public cfixcc::TestFixture
{
public:
  void Sharing()
  {
    SharedCount c1;
    SharedCount c2;

    CFIX_ASSERT (c1.getUseCount() == 1);
    CFIX_ASSERT (c2.getUseCount() == 1);
    CFIX_ASSERT (c1 != c2);

    // Assignment
    c1 = c2;
    CFIX_ASSERT (c1 == c2);
    CFIX_ASSERT (c1.getUseCount() == 2);
    CFIX_ASSERT (c2.getUseCount() == 2);

    // Copy construct
    SharedCount c3(c1);
    CFIX_ASSERT (c1 == c3 && c2 == c3);
    CFIX_ASSERT (c1.getUseCount() == 3);
    CFIX_ASSERT (c2.getUseCount() == 3);
    CFIX_ASSERT (c3.getUseCount() == 3);

    // Lifetime...
    {
      SharedCount c4(c3);
      CFIX_ASSERT (c1 == c4 && c2 == c4 && c3 == c4);
      CFIX_ASSERT (c1.getUseCount() == 4);
      CFIX_ASSERT (c2.getUseCount() == 4);
      CFIX_ASSERT (c3.getUseCount() == 4);
      CFIX_ASSERT (c4.getUseCount() == 4);
    }

    CFIX_ASSERT (c1.getUseCount() == 3);
    CFIX_ASSERT (c2.getUseCount() == 3);
    CFIX_ASSERT (c3.getUseCount() == 3);
  }
};

CFIXCC_BEGIN_CLASS(TestSharedCount)
  CFIXCC_METHOD(Sharing)
CFIXCC_END_CLASS()