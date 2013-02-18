
#include <cfixcc.h>
#include "SharedPointer.h"

class TestSharedPointer : public cfixcc::TestFixture
{
private:
  class SharedClass
  {
  public:
    SharedClass() :
      mId(sNextId),
      mDayTuh(new int[100])
    {
      ++sNextId;
    }

    ~SharedClass()
    {
      delete [] mDayTuh;
    }

  private:
    int* mDayTuh;
    int mId;
    static int sNextId;
  };

public:
  void Sharing()
  {
    SharedPointer<SharedClass> ptr;
    ptr = new SharedClass();
    CFIX_ASSERT (ptr != NULL);

    SharedPointer<SharedClass> ptr2;
    CFIX_ASSERT (ptr != ptr2);

    const char* test = "hello";
    //CFIX_ASSERT (ptr != test);
  }
};

int TestSharedPointer::SharedClass::sNextId = 0;

CFIXCC_BEGIN_CLASS(TestSharedPointer)
  CFIXCC_METHOD(Sharing)
CFIXCC_END_CLASS()