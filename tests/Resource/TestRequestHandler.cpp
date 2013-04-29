#include <cfixcc.h>
#include "IRequest.h"
#include <string.h>

class TestRequest : public cfixcc::TestFixture
{
public:
  class WifiDevice
  {
  public:
    static bool GetInitialized() { return sInitialized; }
    static void Initialize(IRequest* request)
    {
      if (request->type() == IRequest::eInvoke)
      {
        sInitialized = true;
      }
      else
      {
        request->respond()->setFailure();
      }
    }

    const char* getSsid() { return mSsid; }
    void ssid(IRequest* request)
    {
      if (request->type() == IRequest::eSet)
      {
        //mSsid = variant_cast<const char*>(request->getValue());
        Variant arg = request->getArgument();
        arg.convertTo(&mSsid);
      }
      else if (request->type() == IRequest::eGet)
      {
        //request->respond()->write("ssid", mSsid);
      }
      else
      {
        request->respond()->setFailure();
      }
    }

  protected:
    const char* mSsid;
    static bool sInitialized;
  };

  void Simple()
  {
    bool status = false;

    Resources system;
    WifiDevice wifiDevice;

    status = system.add("system.wifi.initialize", &WifiDevice::Initialize);
    CFIX_ASSERT(status == true);

    status = system.add("system.wifi.ssid", &wifiDevice, &WifiDevice::ssid);
    CFIX_ASSERT(status == true);

    status = system.set("system.wifi.ssid", "myWifiNetwork");
    CFIX_ASSERT(status == true);
    CFIX_ASSERT(strcmp(wifiDevice.getSsid(), "myWifiNetwork") == 0);
  }
};

bool TestRequest::WifiDevice::sInitialized = false;

CFIXCC_BEGIN_CLASS(TestRequest)
  CFIXCC_METHOD(Simple)
CFIXCC_END_CLASS()