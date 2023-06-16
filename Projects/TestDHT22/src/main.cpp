#include <Arduino.h>
#include <DHT.h>

#define DHTPIN 12
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

void setup()
{
  Serial.begin(115200);
  dht.begin();

  // put your setup code here, to run once:
}

void loop()
{
  // put your main code here, to run repeatedly:
  float temp = dht.readTemperature(false);
  float hum = dht.readHumidity(false);
  int tempe = ((int)temp);
  int humi = ((int)hum);
  Serial.print("Temp: ");
  Serial.println(tempe);

  Serial.println(1);

  delay(1000);
}