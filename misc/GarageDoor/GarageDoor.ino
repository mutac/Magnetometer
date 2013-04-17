#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <ResourceCollection.h>

char ssid[] = "";
char pass[] = "";
int keyInex = 0;

WiFiServer server(80);

void fatal(int code)
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
  PathCollection<int> paths;
  
  int doo = 10;
  paths.add("test.doo", &doo);
  
  Serial.begin(9600);
  
  if (WiFi.status() == WL_NO_SHIELD)
  {
    Serial.println("WiFi shield not present");
    fatal(911000);
  }
  
  int connectionAttempt = 1;
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print("[");
    Serial.print(connectionAttempt);
    Serial.print("]");
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
   
    WiFi.begin(ssid, pass);
    delay(10000); 
  }
  
  printWiFiStatus();
}

void loop()
{
}

void printWiFiStatus() 
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


