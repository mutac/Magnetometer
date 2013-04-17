#include <cfixcc.h>
#include "IRequest.h"

class TestRequestHandler : public cfixcc::TestFixture
{
public:
  class TestRequest : public IRequest
  {
  public:
    TestRequest() :
      mBegun(false),
      mEnded(false)
    {
    }

    void beginResponse()
    {
      mBegun = true;
    }
    bool begun() { return mBegun; }

    void endResponse()
    {
      mEnded = true;
    }
    bool ended() { return mEnded; }

  private:
    bool mBegun;
    bool mEnded;
  };

  class WifiProperties : public IRequestHandler
  {
  public:
    void ssidHandler(IRequest* request)
    {
      request->beginResponse();
      request->endResponse();
    }
  };

  void Simple()
  {
    ResourceCollection collection;
    WifiProperties wifi;

    RequestHandler* handler = new RequestHandler(&wifi, &WifiProperties::ssidHandler);

    TestRequest test;
    handler->invoke(&test);
    CFIX_ASSERT(test.begun());
    CFIX_ASSERT(test.ended());
  }
};

CFIXCC_BEGIN_CLASS(TestRequestHandler)
  CFIXCC_METHOD(Simple)
CFIXCC_END_CLASS()