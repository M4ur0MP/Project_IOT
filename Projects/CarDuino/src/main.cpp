#include <Arduino.h>
#include <AFMotor.h>
#include <Servo.h>
#include <HCSR04.h>

const int sensor = A0;
unsigned long interval = 1000;
unsigned long previousMillis = 0;

AF_DCMotor Motor1(1);
AF_DCMotor Motor2(2);
AF_DCMotor Motor3(3);
AF_DCMotor Motor4(4);

Servo servo1;
int angulo = 0;
float distance = 0;

void liberar()
{
  Motor1.run(RELEASE);
  Motor2.run(RELEASE);
  Motor3.run(RELEASE);
  Motor4.run(RELEASE);
}

void atras()
{

  Motor1.run(FORWARD);
  Motor2.run(BACKWARD);
  Motor3.run(BACKWARD);
  Motor4.run(FORWARD);
}

void adelante()
{
  Motor1.run(BACKWARD);
  Motor2.run(FORWARD);
  Motor3.run(FORWARD);
  Motor4.run(BACKWARD);
}

void izquierda()
{

  Motor1.run(FORWARD);
  Motor2.run(FORWARD);
  Motor3.run(FORWARD);
  Motor4.run(FORWARD);
}

void derecha()
{

  Motor1.run(BACKWARD);
  Motor2.run(BACKWARD);
  Motor3.run(BACKWARD);
  Motor4.run(BACKWARD);
}
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

void chequearAgain()
{

  if (getDistance() > 30)
  {
    if (angulo == 180)
    {

      if (getDistance() > 30)
      {
        izquierda();
        delay(1000);
        liberar();
      }
      else
      {
        atras();
        delay(1000);
        liberar();
        angulo = 0;
        delay(25);
        chequearAgain();
      }
    }
    derecha();
    delay(1000);
    liberar();
  }
  else
  {
    servo1.write(180);
    delay(15);
    angulo = 180;

    if (getDistance() <= 30)
    {
      atras();
      delay(1000);
      liberar();
      chequearAgain();
    }
  }
}

void setup()
{
  Serial.begin(115200);

  Motor1.setSpeed(110);
  Motor2.setSpeed(110);
  Motor3.setSpeed(110);
  Motor4.setSpeed(110);
  servo1.attach(17);
  servo1.write(90);
  delay(30);
}

void loop()
{

  Serial.println(getDistance());

  delay(20);
  if (getDistance() >= 30.0)
  {
    adelante();
    if (angulo != 90)
    {
      servo1.write(90);
      angulo = 90;
    }
    delay(30);
  }
  else
  {
    atras();
    delay(1000);
    liberar();
    servo1.write(180);
    delay(20);

    if (getDistance() < 30)
    {
      servo1.write(0);
      delay(20);

      if (getDistance() < 30)
      {
        atras();
        delay(2000);
        liberar();
        angulo = 0;
        chequearAgain();
      }
      else
      {
        izquierda();
        delay(1000);
        liberar();
      }
    }
    else
    {
      derecha();
      delay(1000);
      liberar();
      angulo = 180;
    }
  }
}