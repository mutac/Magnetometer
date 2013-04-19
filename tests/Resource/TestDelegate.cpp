
#include <cfixcc.h>
#include "Delegate.h"

class TestDelegate : public cfixcc::TestFixture
{
public:
  class TestObserver
  {
  public:
    TestObserver() :
      mHandled(false)
    {
    }

    void handled()
    {
      mHandled = true;
    }

    bool isHandled() const
    {
      return mHandled;
    }

    void reset()
    {
      mHandled = false;
    }

  private:
    bool mHandled;
  };

  class DeviceProperty
  {
  public:
    void propertyHandler(TestObserver* observer)
    {
      observer->handled();
    }

    static void PropertyHandler(TestObserver* observer)
    {
      observer->handled();
    }
  };

  void Simple()
  {
    DeviceProperty someProperty;
    Delegate<void, TestObserver*> propertyDelegate(&someProperty, &DeviceProperty::propertyHandler);
    Delegate<void, TestObserver*> staticPropertyDelegate(&DeviceProperty::PropertyHandler);

    TestObserver test;

    propertyDelegate.invoke(&test);
    CFIX_ASSERT(test.isHandled());

    test.reset();

    staticPropertyDelegate.invoke(&test);
    CFIX_ASSERT(test.isHandled());
  }
};

CFIXCC_BEGIN_CLASS(TestDelegate)
  CFIXCC_METHOD(Simple)
CFIXCC_END_CLASS()