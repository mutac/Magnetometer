#include <string.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <IRequest.h>
#include <mString.h>

class SerialResponseHandler : public IResponse
{
public:
  SerialResponseHandler(int baud) :
    mFailed(false)
  {
    Serial.begin(baud);
  }
  
  bool write(const char* name, const Variant& val)
  {
    bool succeeded = true;
    mString str;
       
    Serial.print(name);
    Serial.print(": ");
    
    succeeded = val.convertTo(&str);
    if (succeeded) 
    {
      Serial.println(str.c_Str());
    }
    else
    {
      Serial.println("Unknown");
      succeeded = false;
    }
    
    return succeeded;
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
      request->respond()->setFailure("WiFi hardware not found!");
    }
    
    if (strlen(mSsid) == 0)
    {
      request->respond()->setFailure("Invalid ssid");
    }
  
    request->respond()->write("Connecting To", mSsid);
    
    int connectionAttempt = 1;
    for (int attempts = 1; 
      attempts <= mMaxConnectionAttempts && WiFi.status() != WL_CONNECTED; 
      attempts++)
    {
      request->respond()->write("Attempt: ", attempts);
      WiFi.begin(const_cast<char*>(mSsid), const_cast<char*>(mPass));
      delay(10000); 
    }
    
    request->respond()->write("status", WiFi.status() == WL_CONNECTED);
    
    if (WiFi.status() != WL_CONNECTED)
    {
      request->respond()->setFailure("Unable to connect!");
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
    bool succeeded = true;
    
    if (request->type() == IRequest::eSet)
    {
      succeeded = request->getArgument().getValue(&mSsid);
    }
    else if (request->type() == IRequest::eGet)
    {
      request->respond()->write("ssid", mSsid);
    }
    else
    {
      succeeded = false;
    }
    
    if (!succeeded)
    {
      request->respond()->setFailure();
    }
  }
  
  void password(IRequest* request)
  {
    bool succeeded = true;
    
    if (request->type() == IRequest::eSet)
    {
      succeeded = request->getArgument().getValue(&mPass);
    }
    else if (request->type() == IRequest::eGet)
    {
      request->respond()->write("password", mPass);
    }
    else
    {
      succeeded = false;
    }
    
    if (!succeeded)
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
SerialResponseHandler gSerialResponseHandler(9600);
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
  
  gWiFiDevice.addResources(&gSystem);
      
  // 
  // Configure
  //
  
  gSystem.set("system.wifi.ssid", "mySsid");
  gSystem.set("system.wifi.password", "secret");

  gSystem.invoke("system.wifi.connect", &gSerialResponseHandler);
  gSystem.invoke("system.wifi.status", &gSerialResponseHandler);
}

void loop()
{
}


