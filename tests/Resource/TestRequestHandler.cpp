#include <cfixcc.h>
#include "IRequest.h"

class TestRequest : public cfixcc::TestFixture
{
public:
  class ObservingRequest: public IRequest
  {
  public:
    ObservingRequest() :
      mBegun(false),
      mEnded(false)
    {
    }

    void reset()
    {
      mBegun = false;
      mEnded = false;
    }

    void beginResponse()
    {
      mBegun = true;
    }

    bool begun() const
    {
      return mBegun;
    }

    void endResponse()
    {
      mEnded = true;
    }

    bool ended() const
    {
      return mEnded;
    }

  private:
    bool mBegun;
    bool mEnded;
  };

  class TestResources
  {
  public:
    TestResources() :
      mSetInstanceVariable(false)
    {
    }

    void reset()
    {
      mSetInstanceVariable = false;
      mSetStaticVariable = false;
    }

    void instanceProperty(IRequest& request)
    {
      request.beginResponse();
      mSetInstanceVariable = true;
      request.endResponse();
    };

    bool instanceVariableSet() const
    {
      return mSetInstanceVariable;
    }

    static void StaticProperty(IRequest& request)
    {
      request.beginResponse();
      mSetStaticVariable = true;
      request.endResponse();
    }

    static bool StaticVariableSet()
    {
      return mSetStaticVariable;
    }

  private:
    bool mSetInstanceVariable;
    static bool mSetStaticVariable;
  };

  void Simple()
  {
    ResourceCollection system;
    TestResources resources;

    bool success = true;

    success = system.add("system.staticProperty", 
      new RequestHandler(&TestResources::StaticProperty));
    CFIX_ASSERT(success);
    success = system.add("system.instanceProperty", 
      new RequestHandler(&resources, &TestResources::instanceProperty));
    CFIX_ASSERT(success);

    ObservingRequest testRequest;

    success = Request::Invoke(system, "system.staticProperty", testRequest);
    CFIX_ASSERT(success);
    CFIX_ASSERT(testRequest.begun());
    CFIX_ASSERT(testRequest.ended());
    CFIX_ASSERT(TestResources::StaticVariableSet());

    testRequest.reset();
    resources.reset();

    success = Request::Invoke(system, "system.instanceProperty", testRequest);
    CFIX_ASSERT(success);
    CFIX_ASSERT(testRequest.begun());
    CFIX_ASSERT(testRequest.ended());
    CFIX_ASSERT(resources.instanceVariableSet());

    testRequest.reset();
    resources.reset();
  }
};

bool TestRequest::TestResources::mSetStaticVariable = false;

CFIXCC_BEGIN_CLASS(TestRequest)
  CFIXCC_METHOD(Simple)
CFIXCC_END_CLASS()