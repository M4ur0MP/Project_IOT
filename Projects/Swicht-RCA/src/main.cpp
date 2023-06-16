#include <Arduino.h>
#define releR D3
#define releN D4
#define ledC D5
#define led1 D6
#define led2 D7
#define btC D8
int val = 0;

void setup()
{
  Serial.begin(115200);
  pinMode(releR, OUTPUT);
  pinMode(releN, OUTPUT);
  pinMode(ledC, OUTPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(btC, INPUT_PULLUP);
  digitalWrite(releR, LOW);
  digitalWrite(releN, LOW);
}

void loop()
{
  int button = digitalRead(btC);
  if (button == 1)
  {
    if (val == 0)
    {
      digitalWrite(releN, LOW);
      digitalWrite(releR, LOW);
      val = 1;
    }
    if (val == 1)
    {
      digitalWrite(releN, HIGH);
      digitalWrite(releR, HIGH);
      val = 0;
    }
  }
}