/*
 * Example for how to use SinricPro Switch device:
 * - setup a switch device
 * - handle request using callback (turn on/off builtin led indicating device power state)
 * - send event to sinricPro server (flash button is used to turn on/off device manually)
 *
 * If you encounter any issues:
 * - check the readme.md at https://github.com/sinricpro/esp8266-esp32-sdk/blob/master/README.md
 * - ensure all dependent libraries are installed
 *   - see https://github.com/sinricpro/esp8266-esp32-sdk/blob/master/README.md#arduinoide
 *   - see https://github.com/sinricpro/esp8266-esp32-sdk/blob/master/README.md#dependencies
 * - open serial monitor and check whats happening
 * - check full user documentation at https://sinricpro.github.io/esp8266-esp32-sdk
 * - visit https://github.com/sinricpro/esp8266-esp32-sdk/issues and check for existing issues or open a new one
 */

// Uncomment the following line to enable serial debug output
//#define ENABLE_DEBUG

#ifdef ENABLE_DEBUG
#define DEBUG_ESP_PORT Serial
#define NODEBUG_WEBSOCKETS
#define NDEBUG
#endif

#include <Arduino.h>
#ifdef ESP8266
#include <ESP8266WiFi.h>
#endif
#ifdef ESP32
#include <WiFi.h>
#endif

#include "SinricPro.h"
#include "SinricProSwitch.h"

#define WIFI_SSID "SH"
#define WIFI_PASS "noteaprendaslacontrasenia"
#define APP_KEY "11e64a80-4096-4b69-af72-a051d3020173"                                         // Should look like "de0bxxxx-1x3x-4x3x-ax2x-5dabxxxxxxxx"
#define APP_SECRET "006a4009-a80f-4ffe-a513-5063be99f934-af11fbc3-de4d-4b4e-8d9a-7bcdd6b02779" // Should look like "5f36xxxx-x3x7-4x3x-xexe-e86724a9xxxx-4c4axxxx-3x3x-x5xe-x9x3-333d65xxxxxx"
#define luzL "60a9463f2a231603cf2618a1"                                                        // Should look like "5dc1564130xxxxxxxxxxxxxx"
#define luzR "60a9469d8cf8a303b9394086"
#define luzC "61cd74920df86e5c8fef7d32"
#define BAUD_RATE 115200 // Change baudrate to your need

#define BUTTON_PIN 0 // GPIO for BUTTON (inverted: LOW = pressed, HIGH = released)
#define LUZR D3      // GPIO for LED (inverted)
#define LUZL D4
#define LUZC D5

bool myPowerState = false;
unsigned long lastBtnPress = 0;

/* bool onPowerState(String deviceId, bool &state)
 *
 * Callback for setPowerState request
 * parameters
 *  String deviceId (r)
 *    contains deviceId (useful if this callback used by multiple devices)
 *  bool &state (r/w)
 *    contains the requested state (true:on / false:off)
 *    must return the new state
 *
 * return
 *  true if request should be marked as handled correctly / false if not
 */
bool onPowerState(const String &deviceId, bool &state)
{
  if (deviceId == luzL)
  {
    Serial.printf("Device %s turned %s (via SinricPro) \r\n", deviceId.c_str(), state ? "on" : "off");
    myPowerState = state;
    digitalWrite(LUZL, myPowerState ? LOW : HIGH);
    return true; // request handled properly
  }
  else if (deviceId == luzR)
  {
    Serial.printf("Device %s turned %s (via SinricPro) \r\n", deviceId.c_str(), state ? "on" : "off");
    myPowerState = state;
    digitalWrite(LUZR, myPowerState ? LOW : HIGH);
    return true; // request handled properly
  }
  else if (deviceId == luzC)
  {
    Serial.printf("Device %s turned %s (via SinricPro) \r\n", deviceId.c_str(), state ? "on" : "off");
    myPowerState = state;
    digitalWrite(LUZC, myPowerState ? LOW : HIGH);
    return true; // request handled properly
  }
  else
  {
    return false;
  }
}

void handleButtonPress()
{
  unsigned long actualMillis = millis(); // get actual millis() and keep it in variable actualMillis
  if (digitalRead(BUTTON_PIN) == LOW && actualMillis - lastBtnPress > 1000)
  { // is button pressed (inverted logic! button pressed = LOW) and debounced?
    if (myPowerState)
    { // flip myPowerState: if it was true, set it to false, vice versa
      myPowerState = false;
    }
    else
    {
      myPowerState = true;
    }
    digitalWrite(LUZL, myPowerState ? LOW : HIGH); // if myPowerState indicates device turned on: turn on led (builtin led uses inverted logic: LOW = LED ON / HIGH = LED OFF)

    // get Switch device back
    SinricProSwitch &mySwitch = SinricPro[luzL];
    // send powerstate event
    mySwitch.sendPowerStateEvent(myPowerState); // send the new powerState to SinricPro server
    Serial.printf("Device %s turned %s (manually via flashbutton)\r\n", mySwitch.getDeviceId().c_str(), myPowerState ? "on" : "off");

    lastBtnPress = actualMillis; // update last button press variable
  }
}

// setup function for WiFi connection
void setupWiFi()
{
  Serial.printf("\r\n[Wifi]: Connecting");
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.printf(".");
    delay(250);
  }
  Serial.printf("connected!\r\n[WiFi]: IP-Address is %s\r\n", WiFi.localIP().toString().c_str());
}

// setup function for SinricPro
void setupSinricPro()
{
  // add device to SinricPro
  SinricProSwitch &luzLiving = SinricPro[luzL];
  SinricProSwitch &luzRincon = SinricPro[luzR];
  SinricProSwitch &luzCocina = SinricPro[luzC];

  // set callback function to device
  luzLiving.onPowerState(onPowerState);
  luzRincon.onPowerState(onPowerState);
  luzCocina.onPowerState(onPowerState);

  // setup SinricPro
  SinricPro.onConnected([]()
                        { Serial.printf("Connected to SinricPro\r\n"); });
  SinricPro.onDisconnected([]()
                           { Serial.printf("Disconnected from SinricPro\r\n"); });
  SinricPro.begin(APP_KEY, APP_SECRET);
}

// main setup function
void setup()
{
  pinMode(BUTTON_PIN, INPUT_PULLUP); // GPIO 0 as input, pulled high
  pinMode(LUZL, OUTPUT);             // define LED GPIO as output
  digitalWrite(LUZL, LOW);           // turn off LED on bootup
  pinMode(LUZR, OUTPUT);
  digitalWrite(LUZR, LOW);
  pinMode(LUZC, OUTPUT);
  digitalWrite(LUZC, LOW);

  delay(1000);
  digitalWrite(LUZR, HIGH);
  digitalWrite(LUZL, HIGH);
  digitalWrite(LUZC, HIGH);

  Serial.begin(BAUD_RATE);
  Serial.printf("\r\n\r\n");
  setupWiFi();
  setupSinricPro();
}

void loop()
{
  // handleButtonPress();
  SinricPro.handle();
}