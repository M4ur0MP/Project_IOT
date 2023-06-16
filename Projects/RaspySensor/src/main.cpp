#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <aREST.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Adafruit_Sensor.h>

#define DHTPIN D5
#define DHTTYPE 22
#define MQ A0
#define LISTEN_PORT 4523

DHT dht(DHTPIN, DHTTYPE);
aREST rest = aREST();

const char *ssid = "";
const char *password = "";
const char *nameServer = "sensor_H1";
const char *idServer = "1";

WiFiServer server(LISTEN_PORT);

float gasMQ;
float humidity, temperature;

void setup()
{

    Serial.begin(9600);
    dht.begin();

    rest.variable("temperature", &temperature);
    rest.variable("humidity", &humidity);
    rest.variable("contaminacion", &gasMQ);

    rest.set_id(idServer);
    rest.set_name(nameServer);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("WiFi connected!");

    server.begin();

    Serial.println("");
    Serial.println("Servidor iniciado!");
    Serial.println(WiFi.localIP());
}

void loop()
{

    delay(1000);
    gasMQ = analogRead(MQ);
    humidity = dht.readHumidity();
    temperature = dht.readTemperature();

    WiFiClient client = server.available();

    if (!client)
    {
        return;
    }
    while (!client.available())
    {
        delay(1);
    }
    rest.handle(client);
}
