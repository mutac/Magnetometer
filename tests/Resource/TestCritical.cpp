

#include <cfixcc.h>
#include <Critical.h>

class TestCritical : public cfixcc::TestFixture
{
public:
  void TestSection()
  {
    CriticalSection section;

    section.enter();
    section.exit();
  }

  void TestScope()
  {
    CriticalSection critical;
    CriticalSection::Scope lock(critical);
  }
};

CFIXCC_BEGIN_CLASS(TestCritical)
  CFIXCC_METHOD(TestSection)
  CFIXCC_METHOD(TestScope)
CFIXCC_END_CLASS()