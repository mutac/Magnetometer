#include <string.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <IRequest.h>
#include <mString.h>
#include <ArduinoRequestBindings.h>

class Console
{
public:
  Console(Resources* root = NULL, Stream* stream = NULL) :
    mStream(stream),
    mRoot(root),
    mHandler(),
    mReadingCommand(true),
    mEcho(true)
  {
    mLine.ensureCapacity(80);
  }
  
  void setRoot(Resources* root)
  {
    mRoot = root;
  }
  
  void setStream(Stream* stream)
  {
    mStream = stream;
    mHandler.setPrinter(stream);
  }
  
  void echo(IRequest* request)
  {
    bool succeeded = false;
    
    if (request->type() == IRequest::eSet)
    {
      succeeded = request->getArgument().convertTo(&mEcho);
    }
    else if (request->type() == IRequest::eGet)
    {
      request->sender()->write("echo", mEcho);
      succeeded = true;
    }
    
    if (!succeeded)
    {
      request->sender()->setFailure();
    }
  }
  
  bool addResources(Resources* system)
  {  
    system->add("system.console.echo", this, &Console::echo);
    return true;
  }
  
  bool available() 
  {
    return mStream && mStream->available();
  }
  
  char read()
  {
    if (mStream != NULL)
    {
      return mStream->read();
    }
    else
    {
      return '\0';
    }
  }
  
  void print(const char* str)
  {
    if (mStream != NULL)
    {
      mStream->print(str);
    }
  }
  
  void println(const char* str)
  {
    if (mStream != NULL)
    {
      mStream->println(str);
    }
  }
  
  void serviceInput()
  {
    while (available() && mReadingCommand)
    {
      char in = read();
      if (in == '\n')
      {
        mReadingCommand = false;
      }
      else if (in == '\r')
      {
        // ignore
      }
      else
      {
        mLine.append(in);
      }
    }
  }
  
  void serviceRequest()
  { 
    if (!mReadingCommand)
    {
      bool succeeded = false;
      IRequest::RequestType type;
      mString command;
      mString arg;
      
      succeeded = ParseCommand(mLine, &type, &command, &arg);
      if (!succeeded || command == "?" || command == "help")
      {
        println("Syntax: [get|set] path [arg]");
        println("Paths:");
        for (Resources::Path::Iterator it = mRoot->begin(); 
          it != mRoot->end(); ++it)
        {
          print("  ");
          println(it.path()->c_Str());
        }
        println("");
      }
      else
      {
        if (type == IRequest::eGet)
        {
          if (mEcho)
          {
            print(command.c_Str());
            println(": ");
          }
          succeeded = mRoot->invoke(command.c_Str(), &mHandler);
        }
        else if (type == IRequest::eSet)
        {
          succeeded = mRoot->set(command.c_Str(), arg);
          if (succeeded && mEcho)
          {
            print(command.c_Str());
            print(" <- \"");
            print(arg.c_Str());
            println("\"");
          }
        }
        else
        {
          succeeded = false;
        }
        
        if (!succeeded)
        {
          println("Command failed");
        }
      }
      
      mLine = "";
      mReadingCommand = true;
    }
  }
  
protected:

 static bool ParseCommand(const mString& cmdString, 
   IRequest::RequestType* outType, 
   mString* outCommand,
   mString* outArg)
 {
   IRequest::RequestType type;
   mString::Iterator parts = cmdString.split(" ");
   mString command;
   mString arg;
   
   if (*parts == "get")
   {
     type = IRequest::eGet;
     ++parts;
     *outCommand = *parts;
   }
   else if (*parts == "set")
   {
     type = IRequest::eSet;
     ++parts; 
     *outCommand = *parts;
     
     if (parts != cmdString.end())
     {
       ++parts;
       *outArg = *parts; 
     }
   }
   else
   {
     // Default to get/invoke
     type = IRequest::eGet;
     *outCommand = *parts;
   }
   
   *outType = type;
   
   return !outCommand->empty();
 }

 bool mEcho;
 Stream* mStream;
 PrintingResponseHandler mHandler;
 Resources* mRoot;
 mString mLine;
 bool mReadingCommand;
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
    system->add("system.wifi.rssi", this, &WiFiDevice::rssi);
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
    WiFi.disconnect();
  }
  
  void connect(IRequest* request)
  {
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
    if (request->type() != IRequest::eGet)
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
  
  void rssi(IRequest* request)
  {
    if (request->type() != IRequest::eGet)
    {
      request->sender()->setFailure();
    }
    
    request->sender()->write("Signal Strength (dBm)", WiFi.RSSI());
  }
  
  void ssid(IRequest* request)
  {
    bool succeeded = false;
    
    if (request->type() == IRequest::eSet)
    {
      succeeded = request->getArgument().convertTo(&mSsid);
    }
    else if (request->type() == IRequest::eGet)
    {
      request->sender()->write("ssid", mSsid);
      succeeded = true;
    }
    
    if (!succeeded)
    {
      request->sender()->setFailure();
    }
  }
  
  void password(IRequest* request)
  {
    bool succeeded = false;
    
    if (request->type() == IRequest::eSet)
    {
      succeeded = request->getArgument().convertTo(&mPass);
    }
    else if (request->type() == IRequest::eGet)
    {
      request->sender()->write("password", mPass);
      succeeded = true;
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

class TelnetConsoleServer
{
public:
  TelnetConsoleServer() :
    mServer(23),
    mRoot(NULL)
  {
  }
  
  void setRoot(Resources* root)
  {
    mRoot = root;
  }
  
  bool addResources(Resources* system)
  {
    system->add("system.telnet.start", this, &TelnetConsoleServer::start);
    return true;
  }
  
  void start(IRequest* request)
  {
    mServer.begin();
    request->sender()->write("started", true);
  }
  
  Console available()
  { 
    WiFiClient client = mServer.available();
    
    if (client && client.connected())
    {
      Console telnetClient(mRoot, &client);
      return telnetClient;
    }
    else
    {
      return Console();
    }
  }
  
private:
  WiFiServer mServer;
  Resources* mRoot;
};

Resources gSystem;
WiFiDevice gWiFiDevice;
TelnetConsoleServer gTelnetConsoleServer;
Console gSerialConsole;

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
  Serial.println("?/help");
    
  gSerialConsole.setStream(&Serial);
  gSerialConsole.setRoot(&gSystem);
  
  // 
  // Configure
  //
  
  gSerialConsole.addResources(&gSystem);
  gWiFiDevice.addResources(&gSystem);
  gTelnetConsoleServer.addResources(&gSystem);
}

void loop()
{
  gSerialConsole.serviceRequest();
  
  Console telnetConsole = gTelnetConsoleServer.available();
  telnetConsole.serviceInput();  
  telnetConsole.serviceRequest();
}

void serialEvent() 
{
  gSerialConsole.serviceInput();
}


