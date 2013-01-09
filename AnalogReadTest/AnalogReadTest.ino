
int LightSensorIn = A0; // analog

void setup() 
{
  Serial.begin(9600);
}

void loop()
{
  int lightSensorValue = analogRead(LightSensorIn);
  float voltage = lightSensorValue * (5.0 / 1023.0);
  
  Serial.println(voltage);
  
  delay(500);
}
