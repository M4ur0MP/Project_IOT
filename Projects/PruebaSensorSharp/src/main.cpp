#include <Arduino.h>
const int sensor = A0;
unsigned long interval = 1000;
unsigned long previousMillis = 0;
float distance = 0;

float getDistance()
{
  if (analogRead(sensor) < 80)
  {
    distance = 80;
    return distance;
  }
  else if (analogRead(sensor) > 500)
  {
    distance = 10;
    return distance;
  }
  else
  {
    distance = 4800 / (analogRead(sensor) - 20);
    return distance;
  }
}

void setup()
{
  Serial.begin(115200);

  // put your setup code here, to run once:
}

void loop()
{

  Serial.println(getDistance());
  //distance = distanceSensor.getMedianFilterDistance();
  delay(200);
  // put your main code here, to run repeatedly:
}