
#include <mAtomic.h>
#include <cfixcc.h>

class TestAtomic : public cfixcc::TestFixture
{
public:
  void TestConstruct()
  {
    mAtomic<int> i(10);
    mAtomic<double> d(10.0);
  }

  void TestIncrement()
  {
    mAtomic<int> i(0);

    CFIX_ASSERT(i++ == 0);
    CFIX_ASSERT(i == 1);

    CFIX_ASSERT(++i == 2);
    CFIX_ASSERT(i == 2);
  }

  void TestDecrement()
  {
    mAtomic<int> i(0);

    CFIX_ASSERT(i-- == 0);
    CFIX_ASSERT(i == -1);

    CFIX_ASSERT(--i == -2);
    CFIX_ASSERT(i == -2);
  }

  void TestCast()
  {
    mAtomic<int> atom(-1);
    int i = 0;

    i = atom;
    CFIX_ASSERT(i == -1);
  }
};

CFIXCC_BEGIN_CLASS(TestAtomic)
  CFIXCC_METHOD(TestConstruct)
  CFIXCC_METHOD(TestIncrement)
  CFIXCC_METHOD(TestCast)
CFIXCC_END_CLASS()