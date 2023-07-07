#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <iostream>
#include <string>
#include <sstream>
#include <TimeLib.h>
#include <WiFiUdp.h>

#define rotacionIn A0

// Datos Wifi
const char *ssid = "Guifi_2.4GHz";
const char *password = "noteaprendaslacontrasenia";

// Establecer IP, Puerta de Enlace y Máscara
IPAddress ip(183, 153, 0, 91);
IPAddress gateway(183, 153, 0, 1);
IPAddress subnet(255, 255, 255, 0);

// Puerto del servidor web
ESP8266WebServer server(80);

// Sincronización de fecha
static const char ntpServerName[] = "185.90.148.209";
const int timeZone = 1;
WiFiUDP Udp;
unsigned int localPort = 8888;
time_t getNtpTime();
void sendNTPpacket(IPAddress &address);
time_t prevDisplay = 0;

int countN;
int countS;
float lec;
unsigned long myTime;
unsigned long time1;
unsigned long time2;
float value;
float lectura;
//float lectura1 = 2.23;
//float lectura2 = 1.5;
//float lectura3 = 3.1;
//float gps = 5678123;
//const char *matricula = "GPX5678";


float fmap(float x, float in_min, float in_max, float out_min, float out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

String convertJson(float lectura)
{
    prevDisplay = now();
    char *dt = ctime(&prevDisplay);
    tm *gmtm = gmtime(&prevDisplay);
    dt = asctime(gmtm);
    strtok(dt, "\n");
    std::string s("{\n");
    std::stringstream sc;
    sc << s << " \"Sensor1\": " << lectura << ",\n \"Fecha\": \"" << dt << "\" \n}"; //",\n \"Sensor2\": " << lectura1 << ",\n \"Sensor3\": " << lectura2 << ",\n \"Sensor4\": " << lectura3 << ",\n \"Matricula\": \"GPX5678\",\n \"Ubicacion\": " << gps << 
    std::string sa = sc.str();
    char *a = &(sa[0]);
    printf("%s\n", a);
    return a;
}

void handleRoot()
{
    lectura = analogRead(rotacionIn);               // analogRead(dato) * 3.3 / 1023;
    value = fmap(lectura, 0, 1023, 0.0, 3.3); // cambiar escala a 0.0 - 5.0
    char str[256];
    printf(str, "%.2f", value);
    String a = convertJson(value);
    server.send(200, "text/plain", a);
}


void setup() {
  Serial.begin(115200);
  pinMode(rotacionIn, INPUT);
  time1 = millis();

  WiFi.begin(ssid, password);

    WiFi.mode(WIFI_STA);
    WiFi.config(ip, gateway, subnet);

    Serial.print("Entro While");
    while (WiFi.status() != WL_CONNECTED)
        delay(200);

    // Arrancar el servidor
    server.on("/", handleRoot);
    server.begin();
    Serial.print("Arranca Server");

    Udp.begin(localPort);
    setSyncProvider(getNtpTime);
    setSyncInterval(300);
  }

void loop() {
  server.handleClient();
  lec = analogRead(rotacionIn);
  Serial.println(lec);
  myTime= millis()- time1;

  if(lec < 545){
    Serial.print("Count N: ");
    Serial.println(countN);
    countN++;
  }
  if(lec > 585){
    Serial.print("Count S: ");
    Serial.println(countS);
    countS++;
  }
  
  if (myTime>=1000){
    int v = countN - countS;
    int e = countS - countN;
    if(v > 10 || e > 10){
      countN = 0;
      countS = 0;
    }
    int vue = (countN + countS)/2;
     Serial.println("Vueltas: ");
    Serial.println(vue);
    int vueltas = vueltas + vue;
    vue = 0;
    countS = 0;
    countN = 0;
    delay(100);
    time1 = millis();
   
}
 Serial.print("Time: ");
Serial.println(myTime);
//Serial.println(time);
//Serial.println(millis());
}

/*-------- NTP code ----------*/

const int NTP_PACKET_SIZE = 48;     // NTP time is in the first 48 bytes of message
byte packetBuffer[NTP_PACKET_SIZE]; // buffer to hold incoming & outgoing packets

time_t getNtpTime()
{
    IPAddress ntpServerIP; // NTP server's ip address

    while (Udp.parsePacket() > 0)
        ; // discard any previously received packets
    Serial.println("Transmit NTP Request");
    // get a random server from the pool
    WiFi.hostByName(ntpServerName, ntpServerIP);
    Serial.print(ntpServerName);
    Serial.print(": ");
    Serial.println(ntpServerIP);
    sendNTPpacket(ntpServerIP);
    uint32_t beginWait = millis();
    while (millis() - beginWait < 1500)
    {
        int size = Udp.parsePacket();
        if (size >= NTP_PACKET_SIZE)
        {
            Serial.println("Receive NTP Response");
            Udp.read(packetBuffer, NTP_PACKET_SIZE); // read packet into the buffer
            unsigned long secsSince1900;
            // convert four bytes starting at location 40 to a long integer
            secsSince1900 = (unsigned long)packetBuffer[40] << 24;
            secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
            secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
            secsSince1900 |= (unsigned long)packetBuffer[43];
            return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
        }
    }
    Serial.println("No NTP Response :-(");
    return 0; // return 0 if unable to get the time
}

// send an NTP request to the time server at the given address
void sendNTPpacket(IPAddress &address)
{
    // set all bytes in the buffer to 0
    memset(packetBuffer, 0, NTP_PACKET_SIZE);
    // Initialize values needed to form NTP request
    // (see URL above for details on the packets)
    packetBuffer[0] = 0b11100011; // LI, Version, Mode
    packetBuffer[1] = 0;          // Stratum, or type of clock
    packetBuffer[2] = 6;          // Polling Interval
    packetBuffer[3] = 0xEC;       // Peer Clock Precision
    // 8 bytes of zero for Root Delay & Root Dispersion
    packetBuffer[12] = 49;
    packetBuffer[13] = 0x4E;
    packetBuffer[14] = 49;
    packetBuffer[15] = 52;
    // all NTP fields have been given values, now
    // you can send a packet requesting a timestamp:
    Udp.beginPacket(address, 123); // NTP requests are to port 123
    Udp.write(packetBuffer, NTP_PACKET_SIZE);
    Udp.endPacket();
}