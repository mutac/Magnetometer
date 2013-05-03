#include <string.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <IRequest.h>
#include <mString.h>
#include <ArduinoRequestBindings.h>

class WiFiDevice
{
public:
  WiFiDevice() :
    mSsid(""),
    mPass(""),
    mKeyIndex(0),
    mMaxConnectionAttempts(1)
  {
  }
  
  bool addResources(Resources* system)
  {
    system->add("system.wifi.ssid", this, &WiFiDevice::ssid);
    system->add("system.wifi.password", this, &WiFiDevice::password);
    system->add("system.wifi.connect", this, &WiFiDevice::connect);
    system->add("system.wifi.disconnect", this, &WiFiDevice::disconnect);
    system->add("system.wifi.status", this, &WiFiDevice::status);
    return true;
  }
  
  bool checkHardware()
  {
    return WiFi.status() != WL_NO_SHIELD;
  }
  
  void disconnect(IRequest* request)
  {
    if (request->type() != IRequest::eInvoke)
    {
      request->sender()->setFailure();
    }
    
    WiFi.disconnect();
  }
  
  void connect(IRequest* request)
  {
    if (request->type() != IRequest::eInvoke)
    {
      request->sender()->setFailure();
    }
    
    if (!checkHardware())
    {
      request->sender()->setFailure("WiFi hardware not found!");
    }
    
    if (mSsid.empty())
    {
      request->sender()->setFailure("Invalid ssid");
    }
  
    request->sender()->write("Connecting To", mSsid);
    
    int connectionAttempt = 1;
    for (int attempts = 1; 
      attempts <= mMaxConnectionAttempts && WiFi.status() != WL_CONNECTED; 
      attempts++)
    {
      request->sender()->write("Attempt", attempts);
      WiFi.begin(const_cast<char*>(mSsid.c_Str()), 
        const_cast<char*>(mPass.c_Str()));
      delay(10000); 
    }
    
    request->sender()->write("Connected", WiFi.status() == WL_CONNECTED);
    
    if (WiFi.status() != WL_CONNECTED)
    {
      request->sender()->setFailure("Unable to connect!");
    }
  }
  
  void status(IRequest* request) 
  {
    if (request->type() != IRequest::eInvoke)
    {
      request->sender()->setFailure();
    }
    
    mStringPrinter ip;
    WiFi.localIP().printTo(ip);
    mStringPrinter gateway;
    WiFi.gatewayIP().printTo(gateway);
    
    request->sender()->write("SSID", WiFi.SSID());
    request->sender()->write("Signal Strength (dBm)", WiFi.RSSI());
    request->sender()->write("IP Address", ip.toString());
    request->sender()->write("Encryption", (int)WiFi.encryptionType());
  }
  
  void ssid(IRequest* request)
  {
    bool succeeded = true;
    
    if (request->type() == IRequest::eSet)
    {
      succeeded = request->getArgument().convertTo(&mSsid);
    }
    else if (request->type() == IRequest::eGet)
    {
      request->sender()->write("ssid", mSsid);
    }
    else
    {
      succeeded = false;
    }
    
    if (!succeeded)
    {
      request->sender()->setFailure();
    }
  }
  
  void password(IRequest* request)
  {
    bool succeeded = true;
    
    if (request->type() == IRequest::eSet)
    {
      succeeded = request->getArgument().convertTo(&mPass);
    }
    else if (request->type() == IRequest::eGet)
    {
      request->sender()->write("password", mPass);
    }
    else
    {
      succeeded = false;
    }
    
    if (!succeeded)
    {
      request->sender()->setFailure();
    }
  }
  
private:
  mString mSsid;
  mString mPass;
  int mKeyIndex;
  int mMaxConnectionAttempts;
};

Resources gSystem;
PrintingResponseHandler gPrintResponse(&Serial);
WiFiDevice gWiFiDevice;
WiFiServer gServer(80);

void fatal(unsigned int code)
{
  while(1)
  {
    Serial.print("Fatal: ");
    Serial.println(code);
    delay(5000);
  }
}

void setup()
{   
  Serial.begin(9600);
  Serial.print("In ");
  for (int i = 5; i > 0; i--)
  {
    Serial.print(i);
    Serial.print(" ");
    delay(1000);
  }
  Serial.println("");
      
  // 
  // Configure
  //
  
  gWiFiDevice.addResources(&gSystem);
  
  gSystem.set("system.wifi.ssid", "mySsid");
  gSystem.set("system.wifi.password", "passw0rd");

  gSystem.invoke("system.wifi.connect", &gPrintResponse);
  gSystem.invoke("system.wifi.status", &gPrintResponse);
}

void loop()
{
}


