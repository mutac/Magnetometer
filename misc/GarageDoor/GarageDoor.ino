#include <string.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <IRequest.h>

class SerialResponse : public IResponse
{
public:
  SerialResponse(int baud) :
    mFailed(false)
  {
    Serial.begin(baud);
  }
  
  void write(const char* name, const Variant& val)
  {
    Serial.print(name);
    Serial.print(": ");
  }
  
  void setFailure(const char* reason = "")
  {
    mFailed = true;
    Serial.print("Request failure: ");
    Serial.println(reason);
  }
  
  bool failed() const
  {
    return mFailed;
  }
  
private:
  bool mFailed;
};

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
    system->add("system.wifi.status", this, &WiFiDevice::status);
    return true;
  }
  
  bool checkHardware()
  {
    return WiFi.status() != WL_NO_SHIELD;
  }
  
  void connect(IRequest* request)
  {
    if (request->type() != IRequest::eInvoke)
    {
      request->respond()->setFailure();
    }
    
    if (!checkHardware())
    {
      request->respond()->setFailure();
    }
    
    if (strlen(mSsid) == 0)
    {
      request->respond()->setFailure();
    }
  
    int connectionAttempt = 1;
    for (int attempts = 1; 
      attempts <= mMaxConnectionAttempts && WiFi.status() != WL_CONNECTED; 
      attempts++)
    {
      Serial.print("[");
      Serial.print(connectionAttempt);
      Serial.print("]");
      Serial.print("Attempting to connect to SSID: ");
      Serial.println(mSsid);

      char* ssid = const_cast<char*>(mSsid);
      char* pass = const_cast<char*>(mPass);
      WiFi.begin(ssid, pass);
      delay(10000); 
    }
    
    request->respond()->write("status", WiFi.status() == WL_CONNECTED);
    
    if (WiFi.status() != WL_CONNECTED)
    {
      request->respond()->setFailure();
    }
  }
  
  void status(IRequest* request) 
  {
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());
    
    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);
    
    long rssi = WiFi.RSSI();
    Serial.print("Signal Strength: ");
    Serial.print(rssi);
    Serial.println(" dBm");
  }
  
  void ssid(IRequest* request)
  {
    if (request->type() == IRequest::eSet)
    {
      mSsid = variant_cast<const char*>(request->getValue());
    }
    else if (request->type() == IRequest::eGet)
    {
      request->respond()->write("ssid", mSsid);
    }
    else
    {
      request->respond()->setFailure();
    }
  }
  
  void password(IRequest* request)
  {
    if (request->type() == IRequest::eSet)
    {
      mPass = variant_cast<const char*>(request->getValue());
    }
    else if (request->type() == IRequest::eGet)
    {
      request->respond()->write("passwor", mPass);
    }
    else
    {
      request->respond()->setFailure();
    }
  }
  
private:
  const char* mSsid;
  const char* mPass;
  int mKeyIndex;
  int mMaxConnectionAttempts;
};

Resources gSystem;
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
  
  gWiFiDevice.addResources(&gSystem);
      
  // 
  // Configure
  //
  
  gSystem.set("system.wifi.ssid", "mySsid");
  gSystem.set("system.wifi.password", "secret");

  gSystem.invoke("system.wifi.connect");
  gSystem.invoke("system.wifi.status");
}

void loop()
{
}


