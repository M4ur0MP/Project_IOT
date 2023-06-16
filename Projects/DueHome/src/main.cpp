#include <Arduino.h>
#include <SimpleDHT.h>

// Lector de DHT11 Temperatura y Humedad
int pinDHT11 = 7;
SimpleDHT11 dht11(pinDHT11);
byte temperature = 0;
byte humidity = 0;
long tiempo = 0;

// Pines y Variables de Reles y sensores

int luzL = 22;
int luzO = 23;
int ledpir = 5;
int pir = 28;
int pirState = LOW;

// Strings de estado y lectura
String readString;
String inString = "";
String estadopirsms = String(3);
long int lectura;
String smsrecibido;
long tiempoEsms = 0;

String temphum()
{

  int err = SimpleDHTErrSuccess;
  if ((err = dht11.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess)
  {
    Serial.print("Read DHT11 failed, err=");
    Serial.println(err);
    tiempo = millis();
    // return;
  }
  int temp = ((int)temperature);
  int hum = ((int)humidity);

  String output = "Datos: ";
  output += "Temp: ";
  output += temp;
  output += " C - Hume: ";
  output += hum;
  output += " %";
  return output;
}

void EnviarSMS(String texto)
{
  // Se establece el formato de SMS en ASCII
  Serial1.println("AT+CMGF=1");
  delay(1000);

  // Enviar comando para un nuevos SMS al numero establecido
  Serial1.println("AT+CMGS=\"numero_tel_movil\"");
  delay(1000);

  // Enviar contenido del SMS
  Serial1.print(texto);
  delay(1000);

  // Enviar Ctrl+Z
  Serial1.print((char)26);
  delay(1000);

  Serial.println("Enviando sms...");
}

void encender(String dato)
{

  if (dato == "luzL")
  {
    if (digitalRead(luzL) == LOW)
    {
      digitalWrite(luzL, HIGH);
      Serial.println("Living off");
    }
    else
    {
      digitalWrite(luzL, LOW);
      Serial.println("Living on");
    }
  }

  if (dato == "luzO")
  {
    if (digitalRead(luzO) == LOW)
    {
      digitalWrite(luzO, HIGH);
      Serial.println("Desk off");
    }
    else
    {
      digitalWrite(luzO, LOW);
      Serial.println("Desk On");
    }
  }
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
      else
      {
        digitalWrite(ledpir, LOW);
      }
    }
    else
    { // si esta desactivado
      if (millis() - tiempo > 15000)
      {
        digitalWrite(luzL, HIGH);
        digitalWrite(ledpir, HIGH);
      }
      if (pirState == HIGH) // si previamente estaba encendido
      {
        // Serial.println("Sensor parado");
        pirState = LOW;
      }
    }
  }
}

void RecibirSMS()
{
  Serial1.println("AT+CMGF=1");
  delay(1000);
  Serial1.println("AT+CMGL=\"REC UNREAD\",0");
  delay(1000);
  if (Serial1.available() > 0)
  {
    smsrecibido = Serial1.read();
    Serial.print(smsrecibido);
    EnviarSMS(smsrecibido);
  }
}

void setup()
{
  Serial.begin(115200);
  Serial1.begin(115200);
  while (!Serial)
  {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  pinMode(luzL, OUTPUT);
  pinMode(luzO, OUTPUT);
  pinMode(pir, INPUT);
  pinMode(ledpir, OUTPUT);
  digitalWrite(ledpir, LOW);
  digitalWrite(luzL, HIGH);
  digitalWrite(luzO, HIGH);
  estadopirsms = "Des";
}

void loop()
{

  if (Serial.available() > 0)
  {

    while (Serial.available())
    {
      int inChar = Serial.read();
      if (isDigit(inChar))
      {
        // convert the incoming byte to a char and add it to the string:
        inString += (char)inChar;
      }
    }
    // Serial.print(readString);
    // lectura =inString.toInt;
    // Serial.println(lectura);
    // Serial.println(inString);

    if (inString == "0")
    {
      // Serial.println(inString);
      encender("luzL");
    }
    if (inString == "1")
    {
      // Serial.println(inString);
      encender("luzO");
    }
    if (inString == "2")
    {

      if (estadopirsms == "Arm")
      {
        estadopirsms = "Des";
        Serial.println("Desarm");
      }
      else
      {
        estadopirsms = "Arm";
        Serial.println("Arm");
      }
    }
    if (inString == "3")
    {
      String salida = temphum();
      Serial.println(salida);
    }
    if (inString == "4")
    {
      EnviarSMS("Saldo");
    }
    // inString = "";
  }
  /*if (millis() - tiempoEsms > 60000){
    RecibirSMS();
    tiempoEsms=millis();

  }*/
  inString = "";
  controlPir(estadopirsms);
}
