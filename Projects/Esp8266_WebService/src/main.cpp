#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <SoftwareSerial.h>

const char *ssid = "Guifi_2.4GHz";
const char *password = "noteaprendaslacontrasenia";

// Establecer IP, Puerta de Enlace y Máscara
IPAddress ip(183, 153, 0, 45);
IPAddress gateway(183, 153, 0, 1);
IPAddress subnet(255, 255, 0, 0);

// Puerto del servidor web
ESP8266WebServer server(3838);
SoftwareSerial Sim800;

#define pir D5
#define txSim (13)
#define rxSim (12)
#define BAUD_RATE 115200
#define DHTPIN D2
#define DHTTYPE DHT22

sensor_t sensor;
DHT_Unified dht(DHTPIN, DHTTYPE);
float temp = 0;
float hum = 0;

String estadopirsms = String(3);
String smsrecibido;
long tiempo = 0;
long tiempoTH = 0;
long tiempochecksms = 0;
long tiempoth = 0;
int pirState = 0;

void LeerTyH()
{
  if (millis() - tiempoTH > 15000)
  {
    sensors_event_t event;
    dht.temperature().getEvent(&event);
    if (isnan(event.temperature))
    {
      Serial.println(F("Error reading temperature!"));
    }
    else
    {
      Serial.print(F("Temperature: "));
      Serial.print(event.temperature);
      Serial.println(F("°C"));
      temp = event.temperature;
    }
    // Get humidity event and print its value.
    dht.humidity().getEvent(&event);
    if (isnan(event.relative_humidity))
    {
      Serial.println(F("Error reading humidity!"));
    }
    else
    {
      Serial.print(F("Humidity: "));
      Serial.print(event.relative_humidity);
      Serial.println(F("%"));
      hum = event.relative_humidity;
    }
    tiempoTH = millis();
  }
}

void EnviarSMS(String texto)
{
  // Se establece el formato de SMS en ASCII
  Sim800.println("AT+CMGF=1");
  delay(1000);

  // Enviar comando para un nuevos SMS al numero establecido
  Sim800.println("AT+CMGS=\"099707574\"");
  delay(1000);

  // Enviar contenido del SMS
  Sim800.print(texto);
  delay(1000);

  // Enviar Ctrl+Z
  Sim800.print((char)26);
  delay(1000);

  Serial.println("Enviando sms...");
}

void controlPir(String estado)
{
  if (estado == "Arm")
  {
    int val = digitalRead(pir);

    if (val == HIGH)
    { // si está activado

      if (pirState == LOW)
      { // si previamente estaba apagado
        Serial.println("Sensor activado");
        if (millis() - tiempo > 15000)
        {
          EnviarSMS("Se activo PIR");
          // digitalWrite(luzL, LOW);
          // digitalWrite(ledpir, LOW);
          tiempo = millis();
          pirState = HIGH;
        }
      }
    }
    else
    { // si esta desactivado
      if (millis() - tiempo > 15000)
      {
        if (pirState == HIGH) // si previamente estaba encendido
        {
          // Serial.println("Sensor parado");
          pirState = LOW;
        }
      }
    }
  }
}

void RecibirSMS()
{
  String ms;
  // Sim800.println("AT+CMGF=1");
  // delay(1000);
  Sim800.println("AT+CMGL=\"REC UNREAD\",0");
  delay(1000);

  if (Sim800.available() > 0)
  {
    Serial.println(Sim800.read());
    ms = Sim800.read();
    // yield();

    Serial.print("Esto es ms: " + ms);
    EnviarSMS(ms);
    ms = "";
  }
  else
  {
    Serial.println("No hay sms");
  }
}

void handleRoot()
{
  server.send(200, "text/plain", "Hola Mundo");
}

void encender()
{
  server.send(200, "text/plain", "Encender Living");
  // digitalWrite(Luz, LOW);
}

void apagar()
{
  server.send(200, "text/plain", "http://localhost:8080/");
  // digitalWrite(Luz, HIGH);
}

void setup()
{
  Serial.begin(115200);
  Serial.print("Arranco Serial");
  Sim800.begin(BAUD_RATE, SWSERIAL_8N1, rxSim, txSim, false, 95);
  dht.begin();

  pinMode(pir, INPUT);

  // Conectar a la WiFi
  WiFi.begin(ssid, password);

  // Modo cliente
  WiFi.mode(WIFI_STA);
  WiFi.config(ip, gateway, subnet);

  // Esperar hasta conectar
  Serial.print("Entro While");
  while (WiFi.status() != WL_CONNECTED)
    delay(200);

  Serial.print("Paso While");
  Sim800.println("AT+CMGF=1");

  // Arrancar el servidor
  // server.on("/", handleRoot);

  // server.begin();
  // Serial.print("Arranca Server");

  // Serial.print("Pin mode");
}

void loop()
{
  // Comprobar si estamos conectados
  if (WiFi.status() == WL_CONNECTED)
  {
    // server.handleClient();
    Serial.println("Wifi Contectado");
  }
  else
  {
    Serial.print("no entra al if, Wifi no conectado");
  }
  Serial.println(millis());
  if (millis() - tiempoth < 200000)
  {
    LeerTyH();
    int tempe = ((int)temp);
    int humi = ((int)hum);
    tiempoth = millis();
    Serial.println("Temperatura: " + tempe);
    Serial.println("Humedad: " + humi);
  }
  // if (digitalRead(pir) == HIGH)
  //{
  //  EnviarSMS("Se activo pir");
  // }
  if (millis() - tiempochecksms < 150000)
  {
    RecibirSMS();
    tiempochecksms = millis();
  }
}
