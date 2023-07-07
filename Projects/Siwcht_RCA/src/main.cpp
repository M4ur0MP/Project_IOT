#include <Arduino.h>
#define releR D0
#define ledC D1
#define led1 D2
#define led2 D3
#define btC D8
int val = 0;

void setup()
{
  Serial.begin(115200);
  pinMode(releR, OUTPUT);
  //pinMode(releN, OUTPUT);
  pinMode(ledC, OUTPUT);
  digitalWrite(ledC,HIGH);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(btC, INPUT_PULLUP);
  digitalWrite(releR, LOW);
  //digitalWrite(releN, LOW);
}

void loop()
{
  int button = digitalRead(btC);
  if (button == 1)
  {
      digitalWrite(releR, LOW);
      digitalWrite(led1,HIGH);
      digitalWrite(led2,LOW);
      Serial.print("Prende");
      Serial.println(": Boton En high");
  }else{
    digitalWrite(releR, HIGH);
    digitalWrite(led1,LOW);
      digitalWrite(led2,HIGH);
      Serial.print("Apaga");
      Serial.println(": Boton En Low");
  }
}